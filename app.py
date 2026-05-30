from flask import Flask, render_template, request, redirect, url_for, flash, send_file
import os
from io import BytesIO
import csv

app = Flask(__name__)
app.secret_key = "secure_key_change_me"

# ---------- Admin Credentials ----------
ADMIN_USERNAME = "rohan"
ADMIN_PASSWORD = "teamseven"

# ---------- Files ----------
GUESTS_FILE = "guests.txt"
SLOTS_FILE = "slots.txt"

# ---------- Utility ----------
def ensure_files():
    if not os.path.exists(GUESTS_FILE):
        open(GUESTS_FILE, "a").close()
    if not os.path.exists(SLOTS_FILE):
        with open(SLOTS_FILE, "w") as f:
            for i in range(1, 11):
                t = "VIP" if i <= 3 else "Normal"
                f.write(f"{i} 0 0 {t}\n")

def read_guests():
    guests = []
    with open(GUESTS_FILE, "r") as f:
        for line in f:
            p = line.strip().split()
            if len(p) == 5:
                guests.append({
                    "id": int(p[0]),
                    "name": p[1],
                    "vehicle": p[2],
                    "type": p[3],
                    "invite": p[4],
                    "slot": "-"
                })
    slots = read_slots()
    for s in slots:
        if s["booked"]:
            for g in guests:
                if g["id"] == s["guestID"]:
                    g["slot"] = s["id"]
    return guests

def write_guest(g):
    with open(GUESTS_FILE, "a") as f:
        f.write(f"{g['id']} {g['name']} {g['vehicle']} {g['type']} {g['invite']}\n")

def next_guest_id():
    guests = read_guests()
    return (guests[-1]["id"] + 1) if guests else 1001

def read_slots():
    slots = []
    with open(SLOTS_FILE, "r") as f:
        for line in f:
            p = line.strip().split()
            if len(p) == 4:
                slots.append({
                    "id": int(p[0]),
                    "booked": bool(int(p[1])),
                    "guestID": int(p[2]),
                    "type": p[3]
                })
    return slots

def write_slots(slots):
    with open(SLOTS_FILE, "w") as f:
        for s in slots:
            f.write(f"{s['id']} {1 if s['booked'] else 0} {s['guestID']} {s['type']}\n")

# ---------- Routes ----------
@app.route("/")
def home():
    return render_template("index.html")

@app.route("/guestregister", methods=["GET", "POST"])
def register_guest():
    ensure_files()
    if request.method == "POST":
        g = {
            "id": next_guest_id(),
            "name": request.form["name"].replace(" ", "_"),
            "vehicle": request.form["vehicle"],
            "type": request.form["type"],
            "invite": request.form["invite"]
        }
        write_guest(g)
        flash(f"Guest {g['name']} registered successfully!", "success")
        return redirect(url_for("home"))
    return render_template("guestregister.html")

@app.route("/admin_login", methods=["GET", "POST"])
def admin_login():
    if request.method == "POST":
        username = request.form["adminUser"]
        password = request.form["adminPass"]
        if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
            flash("Login successful!", "success")
            return redirect(url_for("admin_dashboard"))
        else:
            flash("Invalid credentials", "danger")
            return redirect(url_for("admin_login"))
    return render_template("admin_login.html")

@app.route("/admin_dashboard")
def admin_dashboard():
    ensure_files()
    guests = read_guests()
    slots = read_slots()
    return render_template("admin_dashboard.html", guests=guests, slots=slots)

@app.route("/allocate", methods=["POST"])
def allocate_slot():
    vehicle = request.form["vehicle"].strip()
    slot_id = int(request.form["slot"])
    guests = read_guests()
    target = next((g for g in guests if g["vehicle"] == vehicle), None)
    if not target:
        flash("Guest not found.", "danger")
        return redirect(url_for("admin_dashboard"))
    slots = read_slots()
    for s in slots:
        if s["id"] == slot_id:
            if s["booked"]:
                flash("Slot already booked!", "danger")
                return redirect(url_for("admin_dashboard"))
            s["booked"] = True
            s["guestID"] = target["id"]
            write_slots(slots)
            flash(f"Allocated slot {slot_id} to {target['name']}", "success")
            return redirect(url_for("admin_dashboard"))
    flash("Invalid slot ID", "danger")
    return redirect(url_for("admin_dashboard"))

@app.route("/release/<int:sid>")
def release_slot(sid):
    slots = read_slots()
    for s in slots:
        if s["id"] == sid:
            s["booked"] = False
            s["guestID"] = 0
    write_slots(slots)
    flash(f"Slot {sid} released.", "info")
    return redirect(url_for("admin_dashboard"))

@app.route("/add_slot", methods=["POST"])
def add_slot():
    slot_type = request.form["slot_type"]
    slots = read_slots()
    next_id = (slots[-1]["id"] + 1) if slots else 1
    slots.append({
        "id": next_id,
        "booked": False,
        "guestID": 0,
        "type": slot_type
    })
    write_slots(slots)
    flash(f"Added new {slot_type} slot (ID: {next_id})", "success")
    return redirect(url_for("admin_dashboard"))

@app.route("/download")
def download():
    guests = read_guests()  # get all guests
    output = BytesIO()
    writer = csv.writer(output)
    
    # Write CSV headers
    writer.writerow(["ID", "Name", "Vehicle", "Type", "Invite", "Slot"])
    
    # Write guest data safely
    for g in guests:
        guest_id = g.get("id", g[0] if isinstance(g, list) else "")
        name = g.get("name", g[1] if isinstance(g, list) else "")
        vehicle = g.get("vehicle", g[2] if isinstance(g, list) else "")
        g_type = g.get("type", g[3] if isinstance(g, list) else "")
        invite = g.get("invite", g[4] if isinstance(g, list) else "")
        slot = g.get("slot", "-")
        writer.writerow([guest_id, name, vehicle, g_type, invite, slot])
    
    output.seek(0)
    return send_file(output, as_attachment=True, download_name="guests.csv", mimetype="text/csv")


# Optional: Search route
@app.route("/search", methods=["POST"])
def search():
    term = request.form["term"].lower()
    guests = [g for g in read_guests() if term in g["name"].lower() or term in g["vehicle"].lower()]
    slots = read_slots()
    flash(f"Search results for '{term}'", "info")
    return render_template("admin_dashboard.html", guests=guests, slots=slots)
@app.route("/help")
def help_page():
    return render_template("help.html")

# ---------- Run App ----------
if __name__ == "__main__":
    ensure_files()
    app.run(debug=True)
