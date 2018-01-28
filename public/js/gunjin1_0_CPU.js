$(document).ready(function(){

//var socket = io();

//imgはkomaの6倍になるようにする。
var imgWidth = 1200;
var imgHeight = 1200;
var komaWidth = 200;
var komaHeight = 200;
var dan = 8;
var suji = 6;

var mathfloor = Math.floor;

var piece = [];
var img;
var cnvsList = [];
var ctxList = [];
var kifu;


var nanteme;

var ratio = window.innerHeight / 626 * 0.9;

$("#field").attr("width", 412 * ratio);
$("#field").attr("height", 626 * ratio);

var canvas_width = $("#field").width();
var canvas_height = $("#field").height();


var komaSize = 67 / 412 * canvas_width;
$(".cell, .emphasis").attr("width", komaSize);
$(".cell, .emphasis").attr("height", komaSize);
$(".shireibu").attr("width", komaSize * 2);
$(".shireibu").attr("height", komaSize);




var board = [
            [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15,16],
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

var num = 0;


var kyokumen;
var emphasis = [];
var empCnvsList = [];
var empCtxList = [];
var komaZenbu = 23;
var komadaiBoard = [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15,16];

var gameChu = false;
var mySengo = 0;

var SENGO = {
    SENTE: 1,
    GOTE:2
}

var ROOMSTATE = {
    EMPTY: 0,
    WAITING: 1,
    HAITIMODE: 2,
    BATTLE: 3,
    BATTLEFINISH: 4
}

var waiting = false;


function DrawKyokumen(){
	if(mySengo == SENGO.GOTE) {
		kyokumen.board = kyokumen.GetGoteBoard(true);
		kyokumen.deadKomas = kyokumen.GetGoteDeadKomas();
	}
	clearAllEmpCanvas();
	for (var dan = 1; dan <= 8; dan++) {
	    for (var suji = 1; suji <= 6; suji++) {
	        DrawIndex(ctxList[dan][suji], piece[kyokumen.board[dan][suji]]);
	    }
	}
    $("#komatemae").empty();
    $("#komaushiro").empty();

    kyokumen.deadKomas = kyokumen.DeadKomasToString();

    for(var i=0;i<kyokumen.deadKomas[0].length;i++){
    	$("#komatemae").append(kyokumen.deadKomas[0][i] + "<br>");
    }
    for(var i=0;i<kyokumen.deadKomas[1].length;i++){
    	$("#komaushiro").append(kyokumen.deadKomas[1][i] + "<br>");
    }

};
function DrawKyokumen_Haiti(){
	clearAllEmpCanvas();
	for (var dan = 1; dan <= 8; dan++) {
	    for (var suji = 1; suji <= 6; suji++) {
	        DrawIndex(ctxList[dan][suji], piece[kyokumen.board[dan][suji]]);
	    }
	}
};

function ToIndexPage(){
	window.location.href = "./index.html";
}


function DrawIndex(ctx, index) {
    ctx.clearRect(0, 0, komaSize, komaSize);
    var x = komaWidth * (index % mathfloor(imgWidth / komaWidth));
    var y = komaHeight * mathfloor(index / mathfloor(imgWidth / komaWidth));
    ctx.drawImage(img, x, y, komaWidth, komaHeight, 0, 0, komaSize, komaSize);
}

function OnDragOver(e) {
    e.preventDefault();
}

//jQuery UI用の書き直されたイベントハンドラ
function OnDrop(e, ui) {

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
	console.log(moto);
	console.log(saki);
    //ドロップ先がcanvasであり、元が空でなく、同じところにドロップしてないなら
    if (saki != null && (kyokumen.board[moto.dan][moto.suji] != 0) && !(saki.suji == moto.suji && saki.dan == moto.dan)) {
		console.log(gameChu);
        clearAllEmpCanvas();
        if (gameChu) {
            //対局中
            if (kyokumen.teban == mySengo) {
                var movableDomain = kyokumen.GetMovableDomain(moto);
                if (movableDomain[saki.dan][saki.suji] == 1) {
                    var te = { From: moto, To: saki, komaInf: kyokumen.board[moto.dan][moto.suji] };
                    if (mySengo == SENGO.GOTE) {
                        te = kyokumen.TeToGote(te);
                    }
					gm.SendTe(te);
					console.log("hoge");
                }
            }
        } else {

            //配置モード


            DrawIndex(motoCtx, piece[0]);

            //駒台から
            if (moto.dan == 0) {
                //console.log(board);
                //console.log(moto);
                //console.log(saki);
                //console.log(moto.suji);
                //console.log(board[saki.dan][saki.suji]);

                if (board[saki.dan][saki.suji] != 0) {

                    var sakipiece = board[saki.dan][saki.suji];

                    for (var i = 1; i <= komadaiBoard.length; i++) {
                        if (board[0][i] == 0) {

                            board[0][i] = sakipiece;
                            DrawIndex(ctxList[0][i], piece[sakipiece]);
                            break;
                        }
                    }
                    board[saki.dan][saki.suji] = board[moto.dan][moto.suji];
                    board[moto.dan][moto.suji] = 0;
                    DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);

                } else {
                    board[saki.dan][saki.suji] = board[moto.dan][moto.suji];
                    board[moto.dan][moto.suji] = 0;
                    DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);
                    DrawIndex(motoCtx, piece[0]);
                }
                //駒台へ
            } else if (saki.dan == 0) {
                //console.log(moto);
                //console.log(saki);
                //console.log(board[moto.dan][moto.suji]);
                //console.log(saki.dan);

                DrawIndex(sakiCtx, piece[board[moto.dan][moto.suji]]);
                board[moto.dan][moto.suji] = 0;
                DrawIndex(motoCtx, piece[board[moto.dan][moto.suji]]);
                //そうでないなら
            } else {
                //console.log(moto);
                //console.log(saki);
                //console.log(board[moto.dan][moto.suji]);
                //console.log(board[saki.dan][saki.suji]);

                //ドロップ先に駒があるなら駒台へ戻す
                if (board[saki.dan][saki.suji] != 0) {
					console.log("debug001");
					console.log(saki);
                    var sakipiece = board[saki.dan][saki.suji];

                    for (var i = 1; i <= komadaiBoard.length; i++) {
                        if (board[0][i] == 0) {

                            board[0][i] = sakipiece;
                            DrawIndex(ctxList[0][i], piece[sakipiece]);
                            break;
                        }
                    }
                    board[saki.dan][saki.suji] = board[moto.dan][moto.suji];
                    board[moto.dan][moto.suji] = 0;
                    DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);

                } else {
                    //ないなら入れ替える
                    board[saki.dan][saki.suji] = board[moto.dan][moto.suji];
                    DrawIndex(sakiCtx, piece[board[saki.dan][saki.suji]]);
                    board[moto.dan][moto.suji] = 0;
                    DrawIndex(motoCtx, piece[board[moto.dan][moto.suji]]);
                }

            }
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

class GameData{
	constructor(board, lastTe, teban, deadKomas){
		this.board = board;
		this.lastTe = lastTe;
		this.teban = teban;
		this.deadKomas = deadKomas;
	}
	
}

class GameManager{
	constructor(opened){
		
		this.opened = opened;
		var gameData = {sente: {name: "あなた"}, gote: {name: "CPU"}};
		this.game = new Game(gameData);
		//プレイヤーは先手
		this.cpu = new CPU(); 
		this.kyokumen = kyokumen;
		this.game.SetInitKyokumen(kyokumen.board, this.cpu.kyokumen.board);
		kyokumen.set_board(this.game.GetSenteBoard(!this.opened));
		kyokumen.Start();
		DrawKyokumen();
		console.log(this.game.kyokumen.AllGouhou());
	}
	SendTe(te){
		console.log(te);
		this.game.Fight(te);
		var v = this.game.FinishCheck();
		var gameData = new GameData(this.game.GetSenteBoard(!this.opened), this.game.kyokumen.lastTe, this.game.teban, this.kyokumen.deadKomas);
		console.log("gameData");
		console.log(gameData);
		switch(v){
			
			//対局続行
			case 0:
				this.update_sashite(gameData);
				if(kyokumen.teban == SENGO.GOTE) {
					this.receive_from_cpu();
				}
				break;
			//終局
			case 1:
			case 2:
			case 3:               
				var vicMsg = ["", "先手勝利", "後手勝利", "引き分け"];
                gameData.vicMsg = vicMsg[v];
                console.log(vicMsg[v]);

                gameData.kifu = this.game.kifu;

                gameData.board = this.game.GetSenteBoard(true);        gameChu = false;
				$("#hikiwakemsg").hide();
				$(".gamefinish").show();
				$("#hikiwakebtn").hide();
				$("#touryou").hide();
				//$("#komaoto")[0].play();
				//$(".battlemode").show();
				$("#mybar").show();
				$("#battlejunbi").css("margin-top", "80px");
				$("#komainfobox").empty();

				kifu = gameData.kifu;
				console.log(kifu);
				nanteme = kifu.length - 1;
				$("#nanteme").text(nanteme);
				$("#tesuu").text(nanteme);
				kyokumen.board = kifu[nanteme].board;
				kyokumen.deadKomas = kifu[nanteme].deadKomas;
				kyokumen.Print();
				DrawKyokumen();
				$(".cell").draggable("disable");
				alert(gameData.vicMsg);
                break;
		}
	}
	start(){
		mySengo = 1;
		gameChu = true;
		this.teban = SENGO.SENTE;
        $("#statemsg").text("対局開始");
        $(".haitimode").hide();
        $(".haitiwaiting").hide();
		$("#haiti_rand").hide();
        //$(".battlemode").show();
		$("#statemsg").hide();
	}
	
	update_sashite(gameData){
        kyokumen.set_board(gameData.board);
        kyokumen.lastTe = gameData.lastTe;
        kyokumen.teban = 3 - kyokumen.teban;
        kyokumen.deadKomas = gameData.deadKomas;
        $("#komatemae").empty();
        $("#komaushiro").empty();

        kyokumen.deadKomas = kyokumen.DeadKomasToString();

        var DEADKOMAMIERU = false;

        if(DEADKOMAMIERU){

	        for(var i=0;i<kyokumen.deadKomas[0].length;i++){
	        	$("#komatemae").append(kyokumen.deadKomas[0][i] + "<br>");
	        }
	        for(var i=0;i<kyokumen.deadKomas[1].length;i++){
	        	$("#komaushiro").append(kyokumen.deadKomas[1][i] + "<br>");
	        }
	    }

        clearAllEmpCanvas();

        $("#komaoto")[0].play();

        for (var dan = 1; dan <= 8; dan++) {
            for (var suji = 1; suji <= 6; suji++) {
                DrawIndex(ctxList[dan][suji], piece[kyokumen.board[dan][suji]]);
            }
        }

        var from = kyokumen.lastTe.From;
        var to = kyokumen.lastTe.To;

        //最後に動いた駒を赤色に表示
        if (mySengo == SENGO.GOTE) {
            to.dan = kyokumen.dan + 1 - to.dan;
            to.suji = kyokumen.suji + 1 - to.suji;
        }
        empCtxList[to.dan][to.suji].fillStyle = "rgba(255,0,0,0.6)";
        empCtxList[to.dan][to.suji].fillRect(0, 0, cnvsList[to.dan][to.suji].width, cnvsList[to.dan][to.suji].height);

        //最後に動いた駒がいた場所を薄い赤色に表示
        if (mySengo == SENGO.GOTE) {
            from.dan = kyokumen.dan + 1 - from.dan;
            from.suji = kyokumen.suji + 1 - from.suji;
        }
        empCtxList[from.dan][from.suji].fillStyle = "rgba(255,255,0,0.4)";
        empCtxList[from.dan][from.suji].fillRect(0, 0, cnvsList[from.dan][from.suji].width, cnvsList[from.dan][from.suji].height);
        if (mySengo == kyokumen.teban) {
          $(".cell").draggable("enable");
          $("#hikiwakebtn").show();
          $("#touryou").show();
        }else{
          $(".cell").draggable("disable");
          $("#hikiwakebtn").hide();
          $("#touryou").hide();
        }
	}
	receive_from_cpu(){
		this.cpu.set_board(this.game.GetGoteBoard());
		this.SendTe(this.cpu.think());
	}
}

class CPU{
	constructor(){
		this.kyokumen = new Kyokumen();
		this.kyokumen.SetRandomBoard();
	}
	set_board(board){
		this.kyokumen.set_board(board);
	}
	think(){
		var gouhou = this.kyokumen.AllGouhou();
		var n = Math.floor(Math.random() * gouhou.length);
		return this.kyokumen.TeToGote(gouhou[n]);
	}
}

var Game = (function () {

    var Game = function (gameData) {
        this.sente = {};
        this.gote = {};
        this.sente.name = gameData.sente.name;
        this.gote.name = gameData.gote.name;
        this.sente.board = [];
        this.gote.board = [];

        //棋譜の表現：initBoardを配列の最初に、手数ごとにboardそのものを要素とする配列を作る
        this.kifu = [];

        this.kyokumen = new Kyokumen();
    };

    var p = Game.prototype;
    p.SetInitKyokumen = function (senteBoard,goteBoard) {
        this.kyokumen.CreateInitBoardFromPlayers(senteBoard, goteBoard);
        this.BoardPushToKifu();
    };

    //配置チェックしてtrue,falseで返す
    p.HaitiCheck = function (board) {
        return true;
    }

    //flag:trueであれば全て表
    p.GetSenteBoard = function (flag) {
        return this.kyokumen.GetSenteBoard(flag);
    };
    p.GetGoteBoard = function (flag) {
        return this.kyokumen.GetGoteBoard(flag);
    };
    p.Start = function () {

    };
    p.Fight = function (te) {
        this.kyokumen.Fight(te);
        this.BoardPushToKifu();
    }
    p.FinishCheck = function () {
        return this.kyokumen.FinishCheck();
    }
    p.BoardPushToKifu = function(){

		var temp = [
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

    	var board = this.kyokumen.board;
    	for(var dan=1;dan<=this.kyokumen.dan;dan++){
    		for(var suji=1;suji<=this.kyokumen.suji;suji++){
    			temp[dan][suji] = board[dan][suji];
    		}
    	}
	    var deadKomas = [];
		for(var i=0;i<=1;i++){
			deadKomas[i] = [];
			for(var j=0;j<this.kyokumen.deadKomas[i].length;j++){
				deadKomas[i][j] = this.kyokumen.deadKomas[i][j];
			}
		}
    	this.kifu.push({board:temp,deadKomas:deadKomas});
    };

    p.CreateGameData = function(){


    };


    return Game;
})();
var gm;

onload = function () {
    kyokumen = new Kyokumen();
	console.log("debug ++++");
	kyokumen.PrintAllStrength();
    kyokumen.board = board;

    //駒の内部表現と画像のインデックスの変換配列 piece[内部表現] =画像のインデックス
    piece = [17, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    for (var i = 33; i <= 49; i++) {
        piece[i] = i - 15;
    }

    var field = document.getElementById("field");
    var ctx_field = field.getContext("2d");
    var img_field = new Image();
    img_field.src = "img/field2.png";
    img_field.onload = function () {
        ctx_field.drawImage(img_field, 0, 0, 1235, 1877, 0, 0, canvas_width, canvas_height);
    }

    var cnvs_base = document.getElementsByClassName("cell")[0];

    //盤面をcanvasで埋める
    var b = document.getElementById("board");
    var bW = 1.67*ratio; //boarder width もともと5

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
                            //cnvsList[i][j] = document.getElementsByClassName("shireibu")[0].cloneNode(true);

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

                        cnvsList[i][j].style.top = (352/412*canvas_width + (bW + komaSize) * (i - 5)) + "px";
                    }
                } else {

                    cnvsList[i][j] = cnvs_base.cloneNode(true);
                    if (i <= 4) {
                        cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j)) + "px";
                        cnvsList[i][j].style.top = (bW + (bW + komaSize) * (i - 1)) + "px";
                    } else {
                        cnvsList[i][j].style.left = (bW + (bW + komaSize) * (suji - j)) + "px";
                        cnvsList[i][j].style.top = (352/412*canvas_width + (bW + komaSize) * (i - 5)) + "px";
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
		cnvsList[0][i].style.left = (komaSize * mathfloor((i-1) % 6)) + "px";
		cnvsList[0][i].style.top = (mathfloor((i - 1) / 6) * komaSize) + "px";
        // cnvsList[0][i].style.left = (mathfloor((i - 1) / 9) * komaSize * 2) + "px";
        // cnvsList[0][i].style.top = (komaSize * mathfloor((i - 1) % 9)) + "px";
        cnvsList[0][i].draggable = "true";
        ctxList[0][i] = cnvsList[0][i].getContext("2d");
        komadai.appendChild(cnvsList[0][i]);
    }
	$("#komadai").width(6 * komaSize*1.03);
	$("#komadai").height(4 * komaSize*1.03);

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
        snap: true,
        revertDuration:200
    });


    $("#board .cell").droppable({
        drop: OnDrop
    });
	
	$("#haiti_rand").click(function(){
	    kyokumen.set_board(kyokumen.GetRandomBoard());
		board = kyokumen.board;
        clearAllEmpCanvas();
        for (var i = 1; i <= komaZenbu; i++) {
            DrawIndex(ctxList[0][i], piece[board[0][i]]);
        }
        for (var i = 1; i <= dan; i++) {
            for (var j = 1; j <= suji; j++) {
                DrawIndex(ctxList[i][j], piece[board[i][j]]);
            }
        }
	});

    $(".cell").mousedown(function () {
        if (!gameChu) return;
        if (mySengo != kyokumen.teban) return;
        clearAllEmpCanvas();
        var pos = getIndexInContextList(this.getContext("2d"));
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

    $(".cell").mouseup(function () {
        if (!gameChu) return;
        if (mySengo != kyokumen.teban) return;

        clearAllEmpCanvas();
    });

    $(".cell").click(function () {
        if (gameChu) return;
        clearAllEmpCanvas();
        var pos = getIndexInContextList(this.getContext("2d"));
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

	$(".cell").mouseover(function(){
		if(!gameChu) return;
		$("#komainfobox").empty();
		var pos = getIndexInContextList(this.getContext("2d"));
        if (pos != null) {
        	if(isSelf(kyokumen.board[pos.dan][pos.suji])){
        		var strengthList = kyokumen.GetStrengthList(kyokumen.board[pos.dan][pos.suji]);
        		for(var i=0;i<strengthList.length;i++){
        			$("#komainfobox").append(strengthList[i] + "<br>");
        		}
        	}
        }
	});



	$("#nameform").submit(function(){
		var name = document.forms.nameform.name.value;
	    var roomNum = document.getElementById("roomNum").textContent;

	    var data = { name: name ,roomNum:roomNum};
	    socket.emit("taisenmachi", data);
	    $("#nameinput").hide();
	    $("#waiting").show();
	    waiting = true;
	    return false;
	});

	
    //配置決定、ゲームスタート
    $("#haitikettei").click(function () {
		gm = new GameManager(false);
		gm.start();
    });
	
	//配置決定、相手の駒がわかった状態で対局
    $("#btn_cpu_open").click(function () {
		var opened = true;
		gm = new GameManager(opened);
		gm.start();
    });
	//配置決定、相手の駒がわからない状態で対局
    $("#btn_cpu_hide").click(function () {
		var opened = false;
		gm = new GameManager(opened);
		gm.start();
    });

    //送信した配置を取り消す
    $("#haitichange").click(function(){
    	socket.emit("haitichange",{});
    	$(".haitiwaiting").hide();
    	$(".haitimode").show();
      $(".cell").draggable("enable");
        $("#statemsg").text("初期配置を決めてください。");
    });
    
    $("#touryou").click(function(){
    	if(confirm("投了しますか？")){
    		if(confirm("悔いはないですか？本当に投了しますか？")){
    			socket.emit("touryou", {});
    		}else{
    			alert("じゃあもうちょっと頑張れ");
    		}
    	}
    });
    
    $("#hikiwakebtn").click(function(){
    	if(confirm("引き分け提案しますか？")){
    		$("#hikiwakemsg").text("引き分け提案しています...");
    		$(".cell").draggable("disable");
    		$("#hikiwakebtn").hide();
    		$("#touryou").hide();
    		socket.emit("hikiwake", {});
    	}
    });


    $("#modoru").click(function(){
    	if(nanteme>0) {
    		nanteme--;
        	$("#nanteme").text(nanteme);
	    	kyokumen.board = kifu[nanteme].board;
	        kyokumen.deadKomas = kifu[nanteme].deadKomas;
	    	DrawKyokumen();
    	}
    });

	$("#saishomodoru").click(function(){
		nanteme = 0;
    	$("#nanteme").text(nanteme);
    	kyokumen.board = kifu[nanteme].board;
        kyokumen.deadKomas = kifu[nanteme].deadKomas;
    	DrawKyokumen();
    });

    $("#susumu").click(function(){
    	if(nanteme < kifu.length - 1){
    		nanteme++;
        	$("#nanteme").text(nanteme);
	        $("#komaoto")[0].play();
	    	kyokumen.board = kifu[nanteme].board;
	        kyokumen.deadKomas = kifu[nanteme].deadKomas;
	    	DrawKyokumen(kifu[nanteme]);
    	}
    });
    $("#saigosusumu").click(function(){
		nanteme = kifu.length - 1;
    	$("#nanteme").text(nanteme);
        $("#komaoto")[0].play();
    	kyokumen.board = kifu[nanteme].board;
        kyokumen.deadKomas = kifu[nanteme].deadKomas;
    	DrawKyokumen(kifu[nanteme]);
    });

    //初期化ボタン
    $("#change").click(function () {
		console.log("change clicked");
        clearAllEmpCanvas();
        for (var i = 1; i <= komaZenbu; i++) {
            board[0][i] = komadaiBoard[i];
            DrawIndex(ctxList[0][i], piece[board[0][i]]);
        }
        for (var i = 1; i <= dan; i++) {
            for (var j = 1; j <= suji; j++) {
                board[i][j] = 0;
                DrawIndex(ctxList[i][j], piece[board[i][j]]);
            }
        }
    });




};
});