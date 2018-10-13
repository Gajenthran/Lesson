window.onload = main

var _wW = window.innerWidth;
var _square = new Array(2);
var _sqrSide = 170;
var _sqrCoord = [[0, 0], [-_wW, 0]];

function initSquare() {
    for(let i = 0; i < 2; i++)
        _square[i] = newElement("div", document.body, 
            _sqrCoord[i][0], _sqrCoord[i][1], _sqrSide, _sqrSide, 
            null, { "backgroundColor" : "#a1f442", 
                    "border"          : "solid #44f441 5px" });
}

function moveSquare(dx, dy) {
    for(let i = 0; i < _square.length; i++) {
        _square[i].style.left = (_sqrCoord[i][0] += dx) + "px";
        _square[i].style.top = (_sqrCoord[i][1] += dy) + "px";
    }
}

function isLimit() {
    for(let i = 0; i < _square.length; i++) {
        if(_sqrCoord[i][0] >= _wW)
            _sqrCoord[i][0] = -_wW;
    }
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    moveSquare(3, 0);
    isLimit();
}


function main(){
    initSquare();
    window.requestAnimationFrame(frameloop);
    window.cancelAnimationFrame(frameloop); 
}