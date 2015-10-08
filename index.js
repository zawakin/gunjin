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

function GetClientList() {
    var list = [];
    for (var i = 0; i < clients.length; i++) {
        list.push(clients[i].name);
    }
    return list;
}

io.on("connection", function (socket) {

    console.log("client connected");
    io.emit("change clients", GetClientList());


    socket.on("disconnect", function () {
        
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
            io.to("room" + roomN).emit("gamestart", gunjin.createOneBoard(room.senteBoard, room.goteBoard));
        }

    });
    
});

function ExamineRoomNumber(socket){

}


http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});