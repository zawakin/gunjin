//サーバー

var express = require("express");
var app = express();

var http = require("http").Server(app);

app.use(express.static("public"));
app.set("port", (process.env.PORT || 5000));



app.get("/", function (req, res) {
    res.sendFile("index.html");
});



http.listen(app.get("port"), function () {
    console.log("port = %s", app.get("port"));
});