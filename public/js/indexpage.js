var socket = io();



var roomList,roomNum;

var roomstates = [];



function CreateRooms(rs){
	$(roomList).empty();
	for (var i = 1; i <= roomNum; i++) {
	    var room = document.createElement("li");
	    room.className = "list-group-item";
	    room.innerHTML = "<a href=\"game?r=" + i + "\">" + i + " : "+ rs[i] + "</a>";
	    roomList.appendChild(room);
	}
};

onload = function(){
	roomNum = 100;
	roomList = document.getElementById("roomList");

	for(var i=1;i<=roomNum;i++){
		roomstates[i] = "test";
	}
	CreateRooms(roomstates);
};


socket.on("roomstatechange",function(room){
	roomstates[room.N] = room.state;
	CreateRooms(roomstates);
});