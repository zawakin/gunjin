var socket = io();

var ROOMSTATE = {
    EMPTY: 0,
    WAITING: 1,
    HAITIMODE: 2,
    BATTLE: 3,
    BATTLEFINISH: 4
}

var roomNum;

var roomElements = [];


onload = function(){
	roomNum = 100;
	var List = document.getElementById("roomList");

	for (var i = 1; i <= roomNum; i++) {
	    var room = document.createElement("li");
	    room.className = "list-group-item";
	    room.innerHTML = "<a href=\"game?r=" + i + "\">" + i + " : "+ "取得中" + "</a>";
	    roomElements[i] = room;
	    List.appendChild(room);
	}

	socket.emit("givemerooms",{});

  console.log("debug 2");
};

socket.on("allroomstate",function(rooms){
  for(var i=1;i<=100;i++){
    var room = rooms[i];
    var txt;
  	var vstxt = room.sente + " vs " + room.gote;
  	switch(room.state){
  		case ROOMSTATE.EMPTY:
  			txt = "空室";
  			break;
  		case ROOMSTATE.WAITING:
  			txt = room.waitingname + "　対戦待ち";
  			break;
  		case ROOMSTATE.HAITIMODE:
  		case ROOMSTATE.BATTLE:
  			txt = vstxt + "　対戦中";
  			break;
  		case ROOMSTATE.BATTLEFINISH:
  			txt = vstxt + "　感想戦";
  			break;

  	}
  	var ihtml = "<a href=\"game?r=" + room.N + "\">" + room.N + " : "+ txt + "</a>";
  	$(roomElements[room.N]).html(ihtml);
  }
});

socket.on("roomstatechange",function(room){
  var txt;
	var vstxt = room.sente + " vs " + room.gote;
	switch(room.state){
		case ROOMSTATE.EMPTY:
			txt = "空室";
			break;
		case ROOMSTATE.WAITING:
			txt = room.waitingname + "　対戦待ち";
			break;
		case ROOMSTATE.HAITIMODE:
		case ROOMSTATE.BATTLE:
			txt = vstxt + "　対戦中";
			break;
		case ROOMSTATE.BATTLEFINISH:
			txt = vstxt + "　感想戦";
			break;

	}
	var ihtml = "<a href=\"game?r=" + room.N + "\">" + room.N + " : "+ txt + "</a>";
	$(roomElements[room.N]).html(ihtml);


});
