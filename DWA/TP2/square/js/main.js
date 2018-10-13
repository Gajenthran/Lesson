window.onload = main

var _wW = window.innerWidth;
var _square = new Array(3);
var _sqrSide = 170;
var _sqrCoord = [[0, 0], [-_wW, 0], [0, _sqrSide + 15]];
var _mv2 = "right";

function main(){
    initSquare();
    window.requestAnimationFrame(frameloop);
    window.cancelAnimationFrame(frameloop); 
}

function initSquare() {
    for(let i = 0; i < 3; i++)
        _square[i] = newElement("div", document.body, _sqrCoord[i][0], _sqrCoord[i][1], 
            _sqrSide, _sqrSide, "", { "backgroundColor" : "#ff9900",
                                      "border"          : "5px solid #ea9900"});
}

function moveSquare(dx, dy) {
    for(let i = 0; i < 2; i++) {
        _square[i].style.left = (_sqrCoord[i][0] += dx) + "px";
        _square[i].style.top = (_sqrCoord[i][1] += dy) + "px";
    }
}

function moveSquare2(dx, dy) {
    if(_mv2 == "right")
        _square[2].style.left = (_sqrCoord[2][0] += dx) + "px";

    if(_mv2 == "left")
        _square[2].style.left = (_sqrCoord[2][0] -= dx) + "px";

}
function isLimit() {
    for(let i = 0; i < 2; i++) {
        if(_sqrCoord[i][0] >= _wW)
            _sqrCoord[i][0] = -_wW;
    }

    if(_sqrCoord[2][0] + _sqrSide >= _wW) _mv2 = "left";
    if(_sqrCoord[2][0] <= 0) _mv2 = "right";
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    moveSquare(3, 0);
    moveSquare2(3, 0);
    isLimit();

}
