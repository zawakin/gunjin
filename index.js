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

io.on("connection", function (socket) {

    socket.on("disconnect", function () {
        console.log("client disconnected");
    });

});


http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});