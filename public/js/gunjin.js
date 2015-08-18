

$(function ($) {
    ////gameStage = Haiti, Game, EndGame
    //var gameStage = "Haiti";

    //imgはkomaの6倍になるようにする。
    var imgWidth = 1200;
    var imgHeight = 1200;
    var komaWidth = 200;
    var komaHeight = 200;
    var dan = 8;
    var suji = 6;

    var mathfloor = Math.floor;

    function ShowBoard() {

        var $b = $("#board")
        $b.empty();
        var onmenter = function (e) {
            //   alert();
            $(p).css({
                "left": (e.data._y * komaWidth + 3) + "px",
                "top": (e.data._x * komaHeight + 3) + "px"
            })
        }
        for (var x = 0; x < dan; x++) {
            for (var y = 0; y < suji; y++) {
                var p = piece[board[x][y]].cloneNode(true);

                p.style.left = (y * komaWidth) + "px";
                p.style.top = (x * komaHeight) + "px";
                p.style.position = "absolute";
                ViewportIndex(p.firstChild, imgWidth, imgHeight, { x: 0, y: 0 }, komaWidth, komaHeight, board[x][y]);
                $(p).appendTo($b);
                $(p).on({
                    "click": function () { alert(); }
                });
                (function () {
                    var _X = x, _Y = y;
                    $(p).mouseover(function () {
                        $(p).css({
                            "left": (e.data._Y * komaWidth + 3) + "px",
                            "top": (e.data._X * komaHeight + 3) + "px"
                        })
                    });
                })();

            }
        }

    }

    var piece = [];
    var img;
    var cnvsList = [];
    var ctxList = [];
    var komaSize = 67;


    function DrawIndex(ctx, index) {
        ctx.clearRect(0, 0, komaSize, komaSize);
        var x = komaWidth * (index % mathfloor(imgWidth / komaWidth));
        var y = komaHeight * mathfloor(index / mathfloor(imgWidth / komaWidth));
        ctx.drawImage(img, x, y, komaWidth, komaHeight, 0, 0, komaSize, komaSize);
    }


    var board = [
                    //[64, 64, 64, 64, 64, 64, 64, 64],
                    //[64, 12, 12, 0, 12, 12, 12, 64],
                    //[64, 0, 0, 12, 45, 1, 0, 64],
                    //[64, 8, 9, 14, 14, 14, 11, 64],
                    //[64, 3, 1, 39, 1, 6, 10, 64],
                    //[64, 36, 36, 44, 45, 46, 40, 64],
                    //[64, 12, 12, 12, 12, 12, 12, 64],
                    //[64, 8, 9, 14, 14, 14, 11, 64],
                    //[64, 3, 1, 1, 0, 6, 10, 64],
                    //[64, 64, 64, 64, 64, 64, 64, 64]

                [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15],
                [64, 13, 0, 0, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 13, 0, 13, 0, 13, 0, 64],
                [64, 64, 64, 64, 64, 64, 64, 64]
    ];

    var num = 0;

    function OnDragOver(e) {
        e.preventDefault();
    }

    //jQuery UI用の書き直されたイベントハンドラ
    function OnDrop(e, ui) {

        console.log("drop");
        e.preventDefault();
        //motoはドラッグ元のpos,sakiはドロップ先のpos
        var motoCnvs = ui.helper[0];
        var motoCtx = motoCnvs.getContext("2d");
        var moto = getIndexInContextList(motoCtx);
        //var motoCtx = ctxList[moto.dan][moto.suji];
        //console.log(moto);
        var sakiCnvs = this;
        var sakiCtx = sakiCnvs.getContext("2d");
        var saki = getIndexInContextList(sakiCtx);

        //ドロップ先がcanvasであり、元が空でなく、同じところにドロップしてないなら
        if (saki != null && (board[moto.dan][moto.suji] != 0) && !(saki.suji == moto.suji && saki.dan == moto.dan)) {
            //console.log(saki);
            clearAllEmpCanvas();
            DrawIndex(motoCtx, piece[0]);
            //駒台から
            if (moto.dan == 0) {
                console.log(board);
                console.log(moto);
                console.log(saki);
                console.log(moto.suji);
                console.log(board[saki.dan][saki.suji]);
                board[saki.dan][saki.suji] = board[0][moto.suji];
                DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);
                DrawIndex(motoCtx, piece[0]);
            //駒台へ
            } else if (saki.dan == 0) {
                console.log(moto);
                console.log(saki);
                console.log(board[moto.dan][moto.suji]);
                console.log(saki.dan);

                DrawIndex(sakiCtx, piece[board[moto.dan][moto.suji]]);
                board[moto.dan][moto.suji] = 0;
                DrawIndex(motoCtx, piece[board[moto.dan][moto.suji]]);
            //そうでないなら
            } else {
                console.log(moto);
                console.log(saki);
                console.log(board[moto.dan][moto.suji]);
                console.log(board[saki.dan][saki.suji]);
                board[saki.dan][saki.suji] = board[moto.dan][moto.suji];
                DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);
                board[moto.dan][moto.suji] = 0;
                DrawIndex(motoCtx, piece[board[moto.dan][moto.suji]]);
            }
        }

    }

    //contextを入れるとcellの場所を返す
    function getIndexInContextList(ctx) {
        for (var i = 1; i <= komaZenbu; i++) {
            if (ctx == ctxList[0][i]) {
                return { dan: 0, suji: i };
            }
        }
        for (var i = 1; i <= dan; i++) {
            for (var j = 1; j <= suji; j++) {
                if (ctx == ctxList[i][j]) {
                    return { suji: j, dan: i };
                }
            }
        }
        return null;
    }

    //EmpCanvasをクリア
    function clearAllEmpCanvas() {
        for (var i = 1; i <= dan; i++) {
            for (var j = 1; j <= suji; j++) {
                empCtxList[i][j].clearRect(0, 0, empCnvsList[i][j].width, empCnvsList[i][j].height);
            }
        }
    }

    var kyokumen;
    var emphasis = [];
    var empCnvsList = [];
    var empCtxList = [];
    var komaZenbu = 23;
    var komadaiBoard = [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15];
    onload = function () {

        kyokumen = new Kyokumen();
        kyokumen.board = board;

        kyokumen.Print();
        console.log(kyokumen.GetMovableDomain({ dan: 2, suji: 4 }));
        //駒の内部表現と画像のインデックスの変換配列 piece[内部表現] =　画像のインデックス
        piece = [17, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
        for (var i = 33; i <= 48; i++) {
            piece[i] = i - 15;
        }

        var field = document.getElementById("field");
        var ctx_field = field.getContext("2d");
        img_field = new Image();
        img_field.src = "img/field.png";
        img_field.onload = function () {
            ctx_field.drawImage(img_field, 0, 0, 1235, 1877, 0, 0, 412, 626);
        }

        var cnvs_base = document.getElementsByClassName("cell")[0];

        //盤面をcanvasで埋める
        var b = document.getElementById("board");
        var bW = 1.67 //boarder width もともと5

        //canvasを埋める
        //isCellはcell class か　emp classかを判定する
        function FillCanvasAtBoard(cnvsList, ctxList, cnvs_base,draggable,isCell) {
            for (var i = 1; i <= dan; i++) {
                cnvsList[i] = [];
                ctxList[i] = [];
                for (var j = suji; j >= 1; j--) {
                    if (i == 1 && (j == 3 || j == 4)) {
                        if (j == 3) {

                            if (isCell) {
                                cnvsList[i][j] = cnvs_base.cloneNode(true);

                                cnvsList[i][j].style.left = (mathfloor((komaSize+bW)/2)+bW + (bW + komaSize) * (suji - j - 1)) + "px";
                            } else {
                                cnvsList[i][j] = document.getElementsByClassName("shireibu")[0].cloneNode(true);

                                cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j - 1)) + "px";
                            }
                            cnvsList[i][j].style.top = (bW + (bW + komaSize) * (i - 1)) + "px";
                        }
                        if (j == 4) {

                            cnvsList[i][j] = cnvs_base.cloneNode(true);
                            cnvsList[i][j].style.display = "none";
                        }
                    } else if (i == dan && (j == 3 || j == 4)) {
                        if (j == 3) {

                            cnvsList[i][j] = cnvs_base.cloneNode(true);
                            cnvsList[i][j].style.display = "none";
                        }
                        if (j == 4) {
                            if (isCell) {
                                cnvsList[i][j] = cnvs_base.cloneNode(true);
                                cnvsList[i][j].style.left = (mathfloor((komaSize + bW) / 2) + bW + (bW + komaSize) * (suji - j)) + "px";
                            } else {

                                cnvsList[i][j] = document.getElementsByClassName("shireibu")[0].cloneNode(true);
                                cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j)) + "px";
                            }

                            cnvsList[i][j].style.top = (352 + (bW + komaSize) * (i - 5)) + "px";
                        }
                    } else {

                        cnvsList[i][j] = cnvs_base.cloneNode(true);
                        if (i <= 4) {
                            cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j)) + "px";
                            cnvsList[i][j].style.top = (bW + (bW + komaSize) * (i - 1)) + "px";
                        } else {
                            cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j)) + "px";
                            cnvsList[i][j].style.top = (352 + (bW + komaSize) * (i - 5)) + "px";
                        }
                    }

                    cnvsList[i][j].draggable = draggable;
                    ctxList[i][j] = cnvsList[i][j].getContext("2d");

                    b.appendChild(cnvsList[i][j]);
                }
            }
        }
         FillCanvasAtBoard(empCnvsList, empCtxList, document.getElementsByClassName("emphasis")[0],"true",false);
         FillCanvasAtBoard(cnvsList, ctxList, cnvs_base, "true",true);

        //駒台に駒を追加
        var komadai = document.getElementById("komadai");
        cnvsList[0] = [];
        ctxList[0] = [];
        for (var i = 1; i <= komaZenbu; i++) {
            cnvsList[0][i] = cnvs_base.cloneNode(true);
            cnvsList[0][i].style.left = (mathfloor((i - 1) / 9) * komaSize * 2) + "px";
            cnvsList[0][i].style.top = (komaSize * mathfloor((i - 1) % 9)) + "px";
            cnvsList[0][i].draggable = "true";
            ctxList[0][i] = cnvsList[0][i].getContext("2d");
            komadai.appendChild(cnvsList[0][i]);
        }

        img = new Image();
        img.src = "img/gunjinkoma.png";

        img.onload = function () {
            //駒台上の駒の描画
            for (var i = 1; i <= komaZenbu; i++) {
                DrawIndex(ctxList[0][i], piece[board[0][i]]);
            }

            //フィールドの駒を描画
            for (var i = 1; i <= dan; i++) {
                for (var j = suji; j >= 1; j--) {
                    DrawIndex(ctxList[i][j], piece[board[i][j]]);
                }
            }

        };
        $(".cell").draggable({
            revert: true,
            snap: true
        });


        $("#board .cell").droppable({
            drop: OnDrop
        });

        $(".cell").click(function () {

            clearAllEmpCanvas();
            var pos = getIndexInContextList(this.getContext("2d"));
            console.log(pos);
            if (pos != null) {
                var mikataArray = kyokumen.GetMovableDomain(pos);
                for (var i = 1; i <= dan; i++) {
                    for (var j = 1; j <= suji; j++) {
                        if (mikataArray[i][j] == 1) {

                            empCtxList[i][j].fillStyle = 'rgba(192, 80, 77, 0.7)';
                            empCtxList[i][j].fillRect(0, 0, empCnvsList[i][j].width, empCnvsList[i][j].height);

                        }
                    }
                }
            }
        });

        $("#change").click(function () {
            clearAllEmpCanvas();
            for (var i = 1; i <= dan; i++) {
                for (var j = 1; j <= suji; j++) {
                    board[i][j] = 0;
                    DrawIndex(ctxList[i][j], piece[board[i][j]]);

                }
            }

        });
        $("#sendbtn").click(function () {
            var data = { "board": board };
            $.ajax({
                type: "POST",
                url: "http://localhost:8082/test",
                datatype: "json",
                data: JSON.stringify(data),
                success: function (data) {

                    alert(typeof (data));

                    $("#debug").append("data = " + data + "<br>");
                    $("#debug").append("loadedData = " + JSON.parse(data) + "<br>");
                    console.log(JSON.parse(data));
                }
            });
        });
        $("#getMikata").click(function () {
            var mikataArray = kyokumen.GetMovableDomain({ suji: 2, dan: 4 });
            clearAllEmpCanvas();
            for (var i = 1; i <= dan; i++) {
                for (var j = 1; j <= suji; j++) {
                    if (mikataArray[i][j]==1) {

                        empCtxList[i][j].fillStyle = 'rgba(192, 80, 77, 0.7)';
                        empCtxList[i][j].fillRect(0, 0,cnvsList[i][j].width, cnvsList[i][j].height);

                    }
                }
            }
        });
        $("#empClear").click(function () {
            clearAllEmpCanvas();
        });
    };

});