window.onload = main

var _wW = window.innerWidth;
var _square = new Array(2);
var _sqrSide = 170;
var _sqrCoord = [[0, 0], [-_wW, 0]];

function main(){
	for(let i = 0; i < 2; i++) {
    	_square[i] = initSquare(_sqrCoord[i][0], _sqrCoord[i][1], _sqrSide, _sqrSide, "red");
	}
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
	for(let i = 0; i < 2; i++) {
		_square[i].style.left = (_sqrCoord[i][0] += dx) + "px";
		_square[i].style.top = (_sqrCoord[i][1] += dy) + "px";
	}
}

function isLimit() {
	for(let i = 0; i < 2; i++) {
		if(_sqrCoord[i][0] >= _wW)
			_sqrCoord[i][0] = -_wW;
		console.log(_sqrCoord[0][0] + " - " + _sqrCoord[1][0])
	}
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    moveSquare(4, 0);
    isLimit();

}