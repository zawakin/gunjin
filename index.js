//サーバー

var express = require("express");
var app = express();
var ejs = require("ejs");
var gunjin = require("./module/gunjin.js");

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







var rooms = [];
for(var i=1;i<=100;i++){
    rooms[i] = {waiting:false,num:0, clients:[] ,haitiOk:false};
}

//全体を管理するオブジェクト
var mng = new Manager(100);


io.on("connection", function (socket) {

    console.log("client connected");

    //ソケットが切れたときの処理
    socket.on("disconnect", function () {
        
    });

    //あるソケットが対戦申し込みをしてきたとき
    socket.on("taisenmachi", function (data) {


        var room = rooms[data.roomNum];
        socket.roomN = data.roomNum;
        console.log(data.roomNum + ":" + data.name + " joined.");

        //部屋に誰も入ってなかったら
        if (room.clients.length == 0) {
            room.clients[0] = { id: socket.id, name: data.name };
            socket.join("room" + data.roomNum);
            console.log(data.roomNum + ":" + data.name + " is waiting...");
            room.waiting = true;
        } else {

            //対戦待ちが一人いたら
            if (room.clients.length == 1) {
                room.clients[1] = { id: socket.id, name: data.name };
                socket.join("room" + data.roomNum);

                room.waiting = false;
                console.log(data.roomNum + ": matching success");
                var sente = Math.floor(Math.random() * 2);
                room.sente = room.clients[sente];
                room.gote = room.clients[1 - sente];

                console.log(data.roomNum + ": 先手:" + room.sente.name + " vs 後手:" + room.gote.name);
                io.to("room" + data.roomNum).emit("taisenKettei", room);
            } else {
                //観戦処理...
            }
        }


    });

    //配置受信、チェック
    socket.on("haitikettei", function (board) {
        var room = rooms[socket.roomN];
        var roomN = socket.roomN;

        //配置チェックしレスポンスを返す

        var res = "ok";

        socket.emit("haitikettei", res);

        
        
        if(socket.id == room.sente.id){
            console.log(roomN + ":先手配置完了");
            room.senteBoard = board;
        }else{
            console.log(roomN + ":後手配置完了");
            room.goteBoard = board;
        }

        if (!room.haitiOk) {
            room.haitiOk = true;
        } else {
            console.log(roomN + ":配置完了、対局開始");
            room.board = gunjin.createOneBoard(room.senteBoard, room.goteBoard);

            //io.to("room" + roomN).emit("gamestart", gunjin.createOneBoard(room.senteBoard, room.goteBoard));

            io.to(room.sente.id).emit("gamestart", gunjin.getSenteGamingBoard(room.board));
            io.to(room.gote.id).emit("gamestart", gunjin.getGoteGamingBoard(room.board));
        }

    });
    
});

////gameクラス
//var Game = function () {
//    this.kyokumen = new Kyokumen();

//};

////局面クラス
//var Kyokumen = function () {
//    this.
//}



var Client = (function () {


    var Client = function (socket) {
        this.socket = socket;
    };

    var p = Client.prototype;

    p.SetName = function (name) {
        this.name = name;
    }

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
    var roomN;
    this.roomstate = ROOMSTATE.EMPTY;

    var Room = function (roomN) {
        this.roomN = roomN;
        this.clientList = [];
    };

    var p = Room.prototype;

    //socketを引数にとる
    p.AddClient = function (socket) {
        var client = new Client(socket);
        this.clientList.push(client);
        return this.clientList;
    }

    p.RemoveClient = function (socket) {
        for (var i = 0; i < this.clientList.length; i++) {
            if (this.clientList[i].idIsMine(socket.id)) {
                this.clientList.splice(i, 1);
                break;
            }
        }
        this.MsgToServer("error 削除しようとしたユーザーがいませんでした")
    };

    p.ContainSocket = function (socket) {
        for (var i = 0; i < this.clientList.length; i++) {
            if (this.clientList[i].idIsMine(socket.id)) {
                return true;
            }
        }
        return false;
    };

    p.MsgToServer = function (msg) {
        console.log(this.roomN + " : " + msg);
    }
   
    

    return Room;
})();

var Manager = (function () {
    

    var Manager = function (num) {
        this.rooms = [];
        for (var i = 1; i <= num; i++) {
            this.rooms.push(new Room(i));
        }
    };

    var p = Manager.prototype;

    //0:not found, others : found
    p.WhereSocket = function (socket) {
        for (var i = 1; i <= this.rooms.length; i++) {
            if (rooms[i].ContainSocket(socket)) {
                return i;
            }
        }
        return 0;//not found
    }

    return Manager;
})();




http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});