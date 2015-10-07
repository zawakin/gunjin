//サーバー

var express = require("express");
var app = express();
var ejs = require("ejs");

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
    rooms[i] = {waiting:false,num:0, clients:[]};
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
    	for(var i=0;i<clients.length;i++){
    		if(socket.id==clients[i].id){
    			clients.splice(i,1);
    		}
    	}
        console.log("client disconnected");
    });

    socket.on("taisenmachi", function (data) {
        var room = rooms[data.roomNum];
        console.log(data.roomNum + ":" + data.name + " joined.");
        if (room.clients.length == 0) {
            room.clients[0] = { id: socket.id, name: data.name };
            socket.join("room" + data.roomNum);
            console.log(data.roomNum + ":" + data.name + " is waiting...");
            room.waiting = true;
        } else {
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
            }
        }

        //console.log(data);

    });

    socket.on("taisen", function (name) {
        if (name == "") name = "noname";
        var client = { id: socket.id, name: name };
        var frag = false;
        for (var i = 0; i < clients.length; i++) {
            if (client.id == clients[i].id) {
                frag = true;
                clients[i].name = name;
                break;
            }
        }
        if (!frag) {
        	if(clients.length==0){
            	clients.push(client);
            }else{
            	
            }
        }

        console.log("debug");
        io.emit("change clients", GetClientList());

    });

});


http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});