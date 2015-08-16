//サーバー

var express = require("express");
var app = express();

var http = require("http").Server(app);
var io = require("socket.io")(http);
app.use(express.static("public"));
app.set("port", (process.env.PORT || 5000));



app.get("/", function (req, res) {
    res.sendFile("index.html");
});

var clients = [];

function GetClientList() {
    var list = [];
    for (var i = 0; i < clients.length; i++) {
        list.push(clients[i].name);
    }
    return list;
}

io.on("connection", function (socket) {

    console.log("client connected");
    io.emit("change clients", clients);


    socket.on("disconnect", function () {
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
            clients.push(client);
        }

        console.log("debug");
        io.emit("change clients", GetClientList());

    });

});


http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});