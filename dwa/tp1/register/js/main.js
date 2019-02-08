window.onload = main;

var _table = null;
var _persons = new Array();
var _nbPerson = 500;

function generate() {
    for(let i = 0; i < _nbPerson; i++) {
        let p = {
            firstName   : faker.name.firstName(),
            lastName    : faker.name.lastName(),
            city        : faker.address.city(),
            job         : faker.name.jobTitle()
        };
        _persons.push(p);
    }
}

function initTable() {
    _table = document.createElement("table");
    for(var i = 0; i < _nbPerson; i++) {
        var row = document.createElement("tr");
        for(var property in _persons[i]) {
            var cell = document.createElement("td");
            var text = document.createTextNode(_persons[i][property]);
            cell.appendChild(text);
            row.appendChild(cell);
        }
        _table.appendChild(row);
    }
    document.body.appendChild(_table);
}

function initButtons() {
    var pInfo = Object.keys(_persons[0]);
    var column = Object.keys(_persons[0]).length;
    for(var j = 0; j < column; j++) {
        createButton(pInfo[j]);
    }
}

function createButton(property) {
    var button = document.createElement("button");
    button.id = property;
    button.setAttribute('onclick','sortBy(property);');
    button.onclick = function() {sortBy(property);};
    var text = document.createTextNode(property);
    button.appendChild(text);
    document.body.appendChild(button);
}

function compare(key) {
  return function(a, b) {
    if(!a.hasOwnProperty(key) || !b.hasOwnProperty(key)) {
        return 0; 
    }

    const varA = (typeof a[key] === 'string') ? 
      a[key].toUpperCase() : a[key];
    const varB = (typeof b[key] === 'string') ? 
      b[key].toUpperCase() : b[key];

    let comparison = 0;
    if (varA > varB) {
      comparison = 1;
    } else if (varA < varB) {
      comparison = -1;
    }
    return comparison;
  };
}

function sortBy(property) {
    _persons.sort(compare(property));
    document.body.innerHTML = "";
    initButtons();
    initTable();
}

function main() {
    generate(_nbPerson);
    initButtons();
    initTable();
}
