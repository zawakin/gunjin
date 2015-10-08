module.exports = {
    boardCheck: function (board) {

    },
    createOneBoard: function (senteBoard, goteBoard) {
        var board = [
                    [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 0, 0, 0, 0, 0, 0, 64],
                    [64, 64, 64, 64, 64, 64, 64, 64]
        ];
        for (var dan = 5; dan <= 8; dan++) {
            for (var suji = 1; suji <= 6; suji++) {
                board[dan][suji] = senteBoard[dan][suji];
                board[9 - dan][7 - suji] = goteBoard[dan][suji];
            }
        }
        return board;
    }
}