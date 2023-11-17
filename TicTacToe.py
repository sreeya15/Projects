'''
Name : Sreeya L Karthik
Date : 8 September 2023
Project Title : TicTacToe
Description : The code creates a Tic-Tac-Toe game with a graphical user interface (GUI) that allows two players to take turns. 
	      It checks for win conditions, displays messages for game outcomes, and provides a simple and interactive gaming experience.
'''

from tkinter import *
from tkinter import messagebox											# Import the modules from the Tkinter library

m = Tk()													# Create the main window 
m.title("Tic-Tac-Toe")

clicked = True  												# Flag to keep track of which player's turn
count = 0  													# Counter to keep track of the number of moves 
X = "X"  													# Symbol for the first player "X"
O = "O"  													# Symbol for the second player "O" 

def win():													# Function to check for win condition 
    win_combinations = [(b1, b2, b3), (b4, b5, b6), (b7, b8, b9),						# Winning combinations
                        (b1, b4, b7), (b2, b5, b8), (b3, b6, b9),
                        (b1, b5, b9), (b3, b5, b7)]

    for combo in win_combinations:										# Check each combination for a win
        if combo[0]["text"] == combo[1]["text"] == combo[2]["text"] == X:
            
            combo[0].config(bg="red")										# If "X" wins, change the background color of winning buttons to red
            combo[1].config(bg="red")
            combo[2].config(bg="red")
            
            disable_buttons()  											# Disable all buttons to end the game
            messagebox.showinfo("Tic-Tac-Toe", "Player X won!")  						# Display a message
            return
        elif combo[0]["text"] == combo[1]["text"] == combo[2]["text"] == O:
            
            combo[0].config(bg="red")										# If "O" wins, change the background color of winning buttons to red
            combo[1].config(bg="red")
            combo[2].config(bg="red")
            
            disable_buttons()  											# Disable all buttons to end the game
            messagebox.showinfo("Tic-Tac-Toe", "Player O won!")  						# Display a message
            return

    if count == 9:												# Check for a tie game (all buttons are used and no one wins)
        messagebox.showinfo("Tic-Tac-Toe", "It's a tie!")  							# Display a message


def disable_buttons():												# Function to disable all buttons
    b1.config(state=DISABLED)
    b2.config(state=DISABLED)
    b3.config(state=DISABLED)
    b4.config(state=DISABLED)
    b5.config(state=DISABLED)
    b6.config(state=DISABLED)
    b7.config(state=DISABLED)
    b8.config(state=DISABLED)
    b9.config(state=DISABLED)

def click(b):													# Function to handle button clicks
    global clicked, count, X, O  										# Declare these variables as global to modify them within the function
    if b["text"] == " " and clicked:
        # If the button is not already clicked and it's Player X's turn
        b["text"] = X  												# Set the button's text to "X"
        clicked = False  											# Switch to Player O's turn
        count += 1  												# Increment the move count
        win()  													# Check for a win or a tie
        
    elif b["text"] == " " and not clicked:
        # If the button is not already clicked and it's Player O's turn
        b["text"] = O  												# Set the button's text to "O"
        clicked = True  											# Switch to Player X's turn
        count += 1  												# Increment the move count
        win()  													# Check for a win or a tie
    else:
        messagebox.showerror("Tic-Tac-Toe", "Box is already selected")  					# Show an error message if the button is already selected

# Create buttons for the Tic-Tac-Toe grid
b1 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b1))
b2 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b2))
b3 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b3))
b4 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b4))
b5 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b5))
b6 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b6))
b7 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b7))
b8 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b8))
b9 = Button(m, text=" ", font=("Helvetica", 20), height=3, width=6, bg="AntiqueWhite", command=lambda: click(b9))

b1.grid(row=0, column=0)											# Grid layout for the buttons
b2.grid(row=0, column=1)
b3.grid(row=0, column=2)
b4.grid(row=1, column=0)
b5.grid(row=1, column=1)
b6.grid(row=1, column=2)
b7.grid(row=2, column=0)
b8.grid(row=2, column=1)
b9.grid(row=2, column=2)

m.mainloop()													# Start the main event loop

