window.onload = main

var _wW = window.innerWidth;
var _square = new Array(3);
var _sqrSide = 170;
var _sqrCoord = [[0, 0], [-_wW, 0], [0, _sqrSide]];
var mv2 = "right";

function main(){
    for(let i = 0; i < 3; i++)
        _square[i] = initSquare(_sqrCoord[i][0], _sqrCoord[i][1], _sqrSide, _sqrSide, "#ff9900");


    window.requestAnimationFrame(frameloop);
    window.cancelAnimationFrame(frameloop); 
}

function initSquare(x, y, width, height, color) {
    let square = document.createElement("div");
    square.style.left = x + "px";
    square.style.top = y + "px";
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

function moveSquare2(dx, dy) {
    if(mv2 == "right")
        _square[2].style.left = (_sqrCoord[2][0] += dx) + "px";

    if(mv2 == "left")
        _square[2].style.left = (_sqrCoord[2][0] -= dx) + "px";

}
function isLimit() {
    for(let i = 0; i < 2; i++) {
        if(_sqrCoord[i][0] >= _wW)
            _sqrCoord[i][0] = -_wW;
    }

    if(_sqrCoord[2][0] + _sqrSide >= _wW) mv2 = "left";
    if(_sqrCoord[2][0] <= 0) mv2 = "right";
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    moveSquare(3, 0);
    moveSquare2(8, 0);
    isLimit();

}
