//サーバー

var express = require("express");
var app = express();
var path = require("path");

var http = require("http").Server(app);
var io = require("socket.io")(http);
app.use(express.static("public"));
app.set("port", (process.env.PORT || 5000));



app.get("/", function (req, res) {
    res.sendFile(path.resolve("public/index.html"));
});
app.get("/dojo", function (req, res) {
    res.sendFile(path.resolve("public/dojo.html"));
});

app.get("/game",function(req,res){
    res.sendFile(path.resolve("public/battle.html"));
});

var clients = [];
var games = [];

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