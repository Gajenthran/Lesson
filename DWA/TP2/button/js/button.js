class Button {
    constructor(x, y, w, h, styles) {
        this.element = newElement("div", document.body, x, y, w, h, "", styles);
    }

    changeColor() {
        let r = Math.floor((Math.random() * 255) + 1);
        let g = Math.floor((Math.random() * 255) + 1);
        let b = Math.floor((Math.random() * 255) + 1);
        this.element.style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    }
}

class HoverButton extends Button {
    constructor(x, y, w, h, styles, shadow) {
        super(x, y, w, h, styles);
        this.shadow = shadow;
    }
    
    addShadow() {
        this.element.style.boxShadow = this.shadow + "px " + (this.shadow*2) + "px black";
    }

    removeShadow() {
        this.element.style.boxShadow = "none";
    }
}


class PushButton extends Button {
    constructor(x, y, w, h, styles, shadow) {
        super(x, y, w, h, styles);
        this.shadow = shadow;
    }

    addInsetShadow() {
        this.element.style.boxShadow = "0px 0px 0px 2px inset";
    }

    removeInsetShadow() {
        this.element.style.boxShadow = "none";
    }
}