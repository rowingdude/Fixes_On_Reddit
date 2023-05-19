######################################################################################################################################################
#                                                                                                                                                    #
#                        In this version, I've incorporated the following improvements:                                                             #
#                                                                                                                                                    #
#    Introduced constants for player symbols (PLAYER_X and PLAYER_O) and an empty cell marker (EMPTY_CELL).                                          #
#    Implemented improved input validation using regular expressions to ensure that the user enters two numbers separated by a space.                #
#    Modified the get_starting_board() method to use list comprehension and list slicing to populate the board from user input more succinctly.      #
#    Added the is_valid_move() method to check if a move is valid by verifying that the chosen cell is unoccupied and within the valid range.        #
#    Simplified the win condition checks in the is_over() method using list comprehensions and the all() function.                                   #
#    Created a separate play_game() function to handle the game flow and encapsulate the game loop.                                                  #
#    Removed unnecessary comparisons (== True) in the if statements for checking if the game is over.                                                #
#    Included a if __name__ == '__main__' block to ensure that the game is only played when the script is run directly.                              # 
#                                                                                                                                                    # 
######################################################################################################################################################

import re

class Board:
    EMPTY_CELL = ' '
    PLAYER_X = 'X'
    PLAYER_O = 'O'

    def __init__(self):
        self.board = [[self.EMPTY_CELL] * 3 for _ in range(3)]

    def display(self):
        print("---------")
        for row in self.board:
            print("|", end=" ")
            for element in row:
                print(element, end=" ")
            print("|")
        print("---------")

    def get_starting_board(self):
        u = list(input("> "))
        self.board = [u[i:i+3] for i in range(0, 9, 3)]
        return self.board

    def move(self, player):
        while True:
            move = input('> ')
            if not re.match(r'^[1-3]\s+[1-3]$', move):
                print('You should enter two numbers separated by a space!')
                continue
            i, j = map(int, move.split())
            if not self.is_valid_move(i, j):
                print('Invalid move! Please choose an unoccupied cell within the range of 1 to 3.')
                continue
            break
        self.board[i-1][j-1] = player

    def is_valid_move(self, i, j):
        return 1 <= i <= 3 and 1 <= j <= 3 and self.board[i-1][j-1] == self.EMPTY_CELL

    def is_over(self):
        def check_win(player, cells):
            return all(cell == player for cell in cells)

        for i in range(3):
            if check_win(self.board[i][0], self.board[i]):
                return True
            if check_win(self.board[0][i], [self.board[j][i] for j in range(3)])

        if check_win(self.board[0][0], [self.board[i][i] for i in range(3)]):
            return True
        if check_win(self.board[0][2], [self.board[i][2-i] for i in range(3)]):
            return True

        if all(cell != self.EMPTY_CELL for row in self.board for cell in row):
            print("It's a draw")
            return True

        return False

def play_game():
    board = Board()
    board.display()

    while True:
        board.move(Board.PLAYER_X)
        board.display()
        if board.is_over():
            break

        board.move(Board.PLAYER_O)
        board.display()
        if board.is_over():
            break

if __name__ == '__main__':
    play_game()
