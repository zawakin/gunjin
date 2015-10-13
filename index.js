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



var clients = [];
var games = [];

var rooms = [];
for(var i=1;i<=100;i++){
    rooms[i] = {waiting:false,num:0, clients:[] ,haitiOk:false};
}


io.on("connection", function (socket) {

    console.log("client connected");

    //ソケットが切れたときの処理
    socket.on("disconnect", function () {
        console.log(mng);
        var n = mng.RemoveSocket(socket);
        io.to("room" + n).emit("clientchange", {});
    });

    //対戦申し込みの処理
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

    var Room = function (roomN) {
        this.roomN = roomN;
        this.roomstate = ROOMSTATE.EMPTY;
        this.clientList = [];
    };

    var p = Room.prototype;

    //指定されたソケットを部屋に追加
    p.AddClient = function (socket) {
        var client = new Client(socket);
        this.clientList.push(client);
        return this.clientList;
    }

    //指定されたソケットが存在すれば削除し、いなければエラーをサーバーに送信する関数
    p.RemoveClient = function (socket) {
        for (var i = 0; i < this.clientList.length; i++) {
            if (this.clientList[i].idIsMine(socket.id)) {
                this.clientList.splice(i, 1);
                break;
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

    return Room;
})();

var Manager = (function () {
    var rooms;
    
    var Manager = function (num) {
        rooms = [];
        for (var i = 1; i <= num; i++) {
            rooms.push(new Room(i));
        }
    };

    var p = Manager.prototype;

    //前から順に部屋を検索し、指定されたソケットがあれば部屋番号、無ければ0を返す関数
    p.WhereSocket = function (socket) {

        console.log(rooms);
        console.log(Manager.rooms);
        for (var i = 1; i <= rooms.length; i++) {
            if (rooms[i].ContainSocket(socket)) {
                return i;
            }
        }
        return 0;
    }

    //指定されたソケットが存在すればその部屋からクライアントを削除する関数、戻り値に部屋番号、無ければ0を返す
    p.RemoveSocket = function(socket){
        var n = Manager.WhereSocket(socket);
        if(n==0){
            return 0;
        }else{
            rooms[n].RemoveClient(socket);
            return n;
        }
    }

    console.log(new Manager(1));
    return Manager;
})();



//全体を管理するオブジェクト
var mng = new Manager(100);

http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});