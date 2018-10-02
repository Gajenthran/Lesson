window.onload = main

var _wW = window.innerWidth;
var _square = null;
var _sqrCoord = [3, 0];

function main(){
    _square = initSquare(_sqrCoord[0], _sqrCoord[1], 30, 30, "red");
    window.requestAnimationFrame(frameloop);
	window.cancelAnimationFrame(frameloop); 
}

function initSquare(x, y, width, height, color) {
	let square = document.createElement("div");
	square.style.left = x + "px";
	square.style.top = top + "px";
	square.style.width = width + "px";
	square.style.height = height + "px";
	square.style.position = "absolute";
	square.style.backgroundColor = color;
	document.body.appendChild(square);
	
	return square;
}

function moveSquare(dx, dy) {
	_sqrCoord[0] += dx;
	_sqrCoord[1] += dy;

	_square.style.left = _sqrCoord[0] + "px";
	_square.style.top = _sqrCoord[1] + "px";
	console.log(_square.style.left);
}

function isLimit() {
	if(_sqrCoord[0] >= _wW)
		_sqrCoord[0] = 0; 
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    moveSquare(6, 0);
    isLimit();

}