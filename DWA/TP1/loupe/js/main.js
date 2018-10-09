window.onload = main;

var _canvas = document.getElementById("my_canvas");
var _context;

function main() {
	if(_canvas.getContext)
		_context = _canvas.getContext("2d");


	_context.fillRect(10, 10, 50, 50);

	console.log("main");
	
}