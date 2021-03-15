import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# API KEY !!!

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERM  ANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stock = []
    symbols = []
    summarized = 0
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    grand = cash
    # Creates list of stock own by the user + cash + grand total
    for el in db.execute("SELECT symbol FROM purchases WHERE user_id = ?", session["user_id"]):
        symbol = el["symbol"]
        shares = db.execute("SELECT SUM(amount) FROM purchases WHERE symbol = ? and user_id = ?",
                            symbol, session["user_id"])[0]["SUM(amount)"]
        price = lookup(symbol)["price"]
        name = lookup(symbol)["name"]
        total = price * shares
        if symbol not in symbols:
            stock.append({"symbol": symbol, "name": name, "shares": shares, "price": usd(price), "total": usd(total)})
            symbols.append(symbol)
            summarized += total

        grand = summarized + cash

    return render_template("portfolio.html", stock=stock, cash=usd(cash), grand=usd(grand))


@app.route("/password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change Password"""
    if request.method == "GET":
        return render_template("password.html")

    if request.method == "POST":
        current = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"]
        test_curr = request.form.get("old")
        new = request.form.get("new")
        confirm = request.form.get("confirmation")
        # Checks if the input old password matches the old password in the database

        if not check_password_hash(current, test_curr):
            return apology("Invalid old password")

        # Checks if the new password matches the confirmation
        if new != confirm:
            return apology("New password doesn't match the confirmation")

        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])

        return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        company = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        # Checks whether the input is correct
        if not company:
            return apology("Invalid company symbol")
        if not shares:
            return apology("Invalid share number")
        else:
            for i in shares:
                if i not in "0123456789":
                    return apology("Invalid share number")
        if int(shares) == 0:
            return apology("Invalid share number")

        cash = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]
        shares = int(shares)
        price = float(company["price"])
        payment = shares * price
        if payment > cash:
            return apology("You can't afford this purchase")

        # Updates the database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - payment, session["user_id"])

        db.execute("INSERT INTO purchases (user_id, symbol, price, amount) VALUES (?,?,?,?)",
                   session["user_id"], company["symbol"], price, shares)

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM purchases WHERE user_id = ? ORDER BY time DESC", session["user_id"])
    for t in transactions:
        t["price"] = usd(t["price"])
    return render_template("history.html", transactions=transactions)
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        look = lookup(symbol)
        if look == None:
            return apology("Invalid symbol")
        look["price"] = usd(look["price"])

        return render_template("quoted.html", look=look)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = request.form.get("username")
        # Checks whether the username is valid
        if not username or len(db.execute("SELECT username FROM users WHERE username = ?", username)) != 0:
            return apology("Invalid or already existing username")

        password = request.form.get("password")
        # Checks whether the password is valid
        if not password:
            return apology("Invalid password")

        confirmation = request.form.get("confirmation")
        # Checks whether the password confirmation is valid
        if not confirmation or confirmation != password:
            return apology("Passwords do not match!")

        # Updates the database
        idx = db.execute("INSERT INTO users (username, hash) VALUES(?,?)", username, generate_password_hash(password))
        session["user_id"] = idx

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbols = []
    stock = []
    cash = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]
    # Creates list of stock own by the user
    for el in db.execute("SELECT symbol FROM purchases WHERE user_id = ?", session["user_id"]):
        symbol = el["symbol"]
        shares_owned = db.execute("SELECT SUM(amount) FROM purchases WHERE symbol = ? and user_id = ?",
                                  symbol, session["user_id"])[0]["SUM(amount)"]
        price = lookup(symbol)["price"]
        if symbol not in symbols:
            symbols.append(symbol)
            stock.append({"symbol": symbol, "shares_owned": shares_owned, "price": price})

    if request.method == "GET":
        return render_template("sell.html", stock=stock)

    if request.method == "POST":
        company = request.form.get("symbol")
        shares = request.form.get("shares")
        # Checks whether the company symbol is valid
        if not company:
            return apology("Invalid company symbol")
        # Checks whether the share number is valid
        if not shares:
            return apology("Invalid share number")
        else:
            for i in shares:
                if i not in "0123456789":
                    return apology("Invalid share number")
        if int(shares) == 0:
            return apology("Invalid share number")

        # Checks whether the user owns required number of shares
        for s in stock:
            if s["symbol"] == company:
                if int(shares) > s["shares_owned"]:
                    return apology("You don't own that many shares!")

        total = int(shares) * price
        # Updates the database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + total, session["user_id"])
        db.execute("INSERT INTO purchases (user_id, symbol, price, amount) VALUES (?,?,?,?)",
                   session["user_id"], company, price, -1 * int(shares))

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
