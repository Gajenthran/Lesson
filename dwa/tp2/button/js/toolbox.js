function newElement(nameElement, parent, x, y, w, h, classe, styles) {
	var el = document.createElement(nameElement);
    el.style.position = "absolute";
    el.style.left = x + "px";
    el.style.top = y + "px";
    el.style.width = w + "px";
    el.style.height = h + "px";
    for(var property in styles) el.style[property] = styles[property];
    parent.appendChild(el);

    return el;
}

function on(element, ev, funct) {
	element.addEventListener(ev, funct);
}