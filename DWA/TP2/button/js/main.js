window.onload = main

var _wW = window.innerWidth;
var _wH = window.innerHeight;
var _button = null;
var _buttonStyles = {
    "backgroundColor" : "red",
    "border"          : "none",
    "cursor"          : "pointer",
    "borderRadius"    : "5px",
    "textAlign"       : "center"
};

function main() {
    _button = new PushButton(_wW/2 - 100, _wH/2 - 100, 200 , 100, _buttonStyles, 5);
    window.requestAnimationFrame(frameloop);
    window.cancelAnimationFrame(frameloop);
}

function changeColorButton() {
    console.log(_button.element);
    let r = Math.floor((Math.random() * 255) + 1);
    let g = Math.floor((Math.random() * 255) + 1);
    let b = Math.floor((Math.random() * 255) + 1);
    _button.element.style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
}

function frameloop(time){
    requestAnimationFrame(frameloop);
    on(_button.element, "click", function() {
        _button.changeColor();
    });

/*    on(_button.element, "mouseover", function() {
        _button.addShadow();
    });

    on(_button.element, "mouseout", function() {
        _button.removeShadow();
    }); */

    on(_button.element, "mousedown", function() {
        _button.addInsetShadow();
    });

    on(_button.element, "mouseup", function() {
        _button.removeInsetShadow();
    });


}