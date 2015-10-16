//サーバー

var express = require("express");
var app = express();
var ejs = require("ejs");
var Kyokumen = require("./public/js/GunjinShogiProgram.js");

var http = require("http").Server(app);
var io = require("socket.io")(http);
app.use(express.static("public"));
app.set("port", (process.env.PORT || 5000));

app.engine("ejs",ejs.renderFile);

app.get("/", function (req, res) {
    res.sendFile("index.html");
});

app.get("/game", function (req, res) {
    res.render(__dirname + "/public/taisen.ejs",
        {title:"taitoru", roomNum:req.query.r});

});




io.on("connection", function (socket) {

    console.log("client connected");

    //ソケットが切れたときの処理
    socket.on("disconnect", function () {
        var n = mng.RemoveSocket(socket);
        
        //部屋に参加している人がいなくなったときは部屋を初期化する（要変更）
        if(n!=0){
	    	mng.rooms[n].MsgToServer("誰かがいなくなりましたんで他のユーザーに伝えます");
	    	//mng.rooms[n].Clear();
	        io.to(mng.rooms[n]).emit("clientchange", {});
        }
    });

    //対戦申し込みの処理
    socket.on("taisenmachi", function (data) {
        socket.roomN = data.roomNum;
        var room = mng.rooms[data.roomNum];
		room.MsgToServer(data.name + " が参加しました");
		
		//空室ならば対戦待ちに、対戦待ちがいればマッチング
        switch(room.state){
        	
            case ROOMSTATE.EMPTY:
	        	room.AddClient(socket,data);
	            room.state = ROOMSTATE.WAITING;
	            room.MsgToServer(data.name + " が対戦を待っています...");
        		break;
        		
        	case ROOMSTATE.WAITING:
	        	room.AddClient(socket,data);
                room.state = ROOMSTATE.HAITIMODE;
                room.MsgToServer("マッチング成功");
                
        	    //先後をランダムで決定する
                var gameData = {};
                var sente = Math.floor(Math.random() * 2);
                gameData.sente = room.clientList[sente];
                gameData.gote = room.clientList[1 - sente];
                room.NewGame(gameData);
                room.MsgToServer("先手:" + gameData.sente.name + " vs 後手:" + gameData.gote.name);
                io.to(room.sente.id).emit("SENGO", 1);
                io.to(room.gote.id).emit("SENGO", 2);

                io.to(room.name).emit("taisenKettei", room.game);
        		break;
        	
        	default:
                //観戦処理...
                socket.emit("err","対戦中なので入れません");
        		break;        
        }


    });

    //配置受信、チェック
    socket.on("haitikettei", function (board) {
        var room = mng.rooms[socket.roomN];
        var roomN = socket.roomN;

        //配置チェックしレスポンスを返す

        if (room.game.HaitiCheck(board)) {

            socket.emit("haitikettei", "配置おk");

            switch (socket.id) {
                case room.sente.socket.id:
                    room.MsgToServer("先手 " + room.game.sente.name + " 配置完了");
                    room.game.sente.board = board;
                    room.sente.haitiKanryo = true;
                    break;
                case room.gote.socket.id:
                    room.MsgToServer("後手 " + room.game.gote.name + " 配置完了");
                    room.game.gote.board = board;
                    room.gote.haitiKanryo = true;
                    break;
            }

            if (room.sente.haitiKanryo && room.gote.haitiKanryo) {
                room.state = ROOMSTATE.BATTLE;
                room.MsgToServer("配置完了、対局開始");
                room.game.SetInitKyokumen();

                room.game.Start();
                io.to(room.sente.id).emit("gamestart", room.game.GetSenteBoard());
                io.to(room.gote.id).emit("gamestart", room.game.GetGoteBoard());
            }

        } else {
            socket.emit("haitikettei", "配置やり直し");
        }
    });
    
    socket.on("sashite", function (te) {
        var room = mng.rooms[socket.roomN];
        room.MsgToServer(te.From + " " + te.To);
        room.game.Fight(te);

        var gameData = {};
        gameData.board = room.game.kyokumen.board;
        gameData.Winner = "";

        switch (room.game.FinishCheck()) {
            case 0:
                io.to(room.name).emit("sashite", board);
                break;
            case 1:
                io.to(room.name).emit("gamefinish", gameData);
                room.MsgToServer("senteWin");
                break;
            case 2:
                io.to(room.name).emit("gamefinish", gameData);
                room.MsgToServer("goteWin");
                break;
            case 3:
                io.to(room.name).emit("gamefinish", gameData);
                room.MsgToServer("doro-");
                break;
        }

    });

});

var Game = (function () {

    var Game = function (gameData) {
        this.sente = {};
        this.gote = {};
        this.sente.name = gameData.sente.name;
        this.gote.name = gameData.gote.name;
        this.sente.board = [];
        this.gote.board = [];

        this.kyokumen = new Kyokumen();
    };

    var p = Game.prototype;
    p.SetInitKyokumen = function (senteBoard,goteBoard) {
        this.kyokumen.CreateInitBoardFromPlayers(this.sente.board, this.gote.board);
    };

    //配置チェックしてtrue,falseで返す
    p.HaitiCheck = function (board) {
        return true;
    }

    p.GetSenteBoard = function () {
        return this.kyokumen.GetSenteBoard();
    };
    p.GetGoteBoard = function () {
        return this.kyokumen.GetGoteBoard();
    };
    p.Start = function () {

    };
    p.Fight = function (te) {
        this.kyokumen.Fight(te);
    }
    p.FinishCheck = function (te) {
        return this.kyokumen.FinishCheck();
    }


    return Game;
})();


var Client = (function () {


    var Client = function (socket) {
        this.socket = socket;
        this.id = socket.id;
        this.haitiKanryo = false;
    };

    var p = Client.prototype;

    p.idIsMine = function (id) {
        return this.socket.id == id;
    }

    return Client;
})();


var ROOMSTATE = {
    EMPTY: 0,
    WAITING: 1,
    HAITIMODE: 2,
    BATTLE: 3,
    BATTLEFINISH: 4
}

var Room = (function () {

    var Room = function (roomN) {
        this.roomN = roomN;
        this.roomstate = ROOMSTATE.EMPTY;
        this.clientList = [];
        this.state = ROOMSTATE.EMPTY;
        this.name = "room" + this.roomN;
    };

    var p = Room.prototype;

    //指定されたソケットを部屋に追加
    p.AddClient = function (socket,userData) {
        var client = new Client(socket);
        client.name = userData.name;
        this.clientList.push(client);
	    socket.join(this.name);
        return this.clientList;
    }

    //指定されたソケットが存在すれば削除し、いなければエラーをサーバーに送信する関数
    p.RemoveClient = function (socket) {
        for (var i = 0; i < this.clientList.length; i++) {
            if (this.clientList[i].idIsMine(socket.id)) {
                //console.log(this.clientList);
                this.clientList.splice(i, 1);
                //console.log(this.clientList);
                return;
            }
        }
        this.MsgToServer("error 削除しようとしたユーザーがいませんでした")
    };

    //この部屋に指定したソケットが存在しているかどうかを返す関数
    p.ContainSocket = function (socket) {
        for (var i = 0; i < this.clientList.length; i++) {
            if (this.clientList[i].idIsMine(socket.id)) {
                return true;
            }
        }
        return false;
    };

    //部屋番号を付与してサーバーに送信する関数
    p.MsgToServer = function (msg) {
        console.log(this.roomN + " : " + msg);
    }

    p.NewGame = function (gameData) {
        this.game = new Game(gameData);
        this.sente = gameData.sente;
        this.gote = gameData.gote;
        this.MsgToServer("new game created!");
    };

    return Room;
})();

var Manager = (function () {
	var roomAllNum = 100;
    
    var Manager = function (num) {
        this.rooms = [];
        this.roomNum = num;
        for (var i = 1; i <= this.roomNum; i++) {
            this.rooms[i] = new Room(i);
        }
    };

    var p = Manager.prototype;

    //前から順に部屋を検索し、指定されたソケットがあれば部屋番号、無ければ0を返す関数
    p.WhereSocket = function (socket) {
        for (var i = 1; i <= this.roomNum; i++) {
            if (this.rooms[i].ContainSocket(socket)) {
                return i;
            }
        }
        return 0;
    }

    //指定されたソケットが存在すればその部屋からクライアントを削除する関数、戻り値に部屋番号、無ければ0を返す
    p.RemoveSocket = function(socket){
        var n = this.WhereSocket(socket);
        console.log("debug " +  n);
        if(n==0){
            return 0;
        }else{
            this.rooms[n].RemoveClient(socket);
            return n;
        }
    }
    return Manager;
})();



//全体を管理するオブジェクト
var mng = new Manager(100);

http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});