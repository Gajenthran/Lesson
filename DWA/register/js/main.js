window.onload = main;

var _table = null;
var _tableBody = null;
var _persons = new Array();
var _nbPerson = 3;

function main() {
	generate(_nbPerson);
	displayPerson();
	initButtons();
	initTable();

}

function sortBy(property) {
	_persons.sort(dynamicSort(property));
 	document.body.innerHTML = "";
 	initButtons();
	initTable();
}

function initButtons() {
	var pInfo = Object.keys(_persons[0]);
	var column = objectLength(_persons[0]);
	for(var j = 0; j < column; j++) {
		createButton(pInfo[j]);
	}
}

function initTable() {
	_table = document.createElement("table");
	_tableBody = document.createElement("tbody");
	for(var i = 0; i < _nbPerson; i++) {
		var row = document.createElement("tr");
		for(var property in _persons[i]) {
			var cell = document.createElement("td");
			var text = document.createTextNode(_persons[i][property]);
			cell.appendChild(text);
			row.appendChild(cell);
		}
		_tableBody.appendChild(row);
	}
 
	_table.appendChild(_tableBody);
	document.body.appendChild(_table);
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


function objectLength(obj) {
	var count = 0;
	for(var property in obj) {
		if(Object.prototype.hasOwnProperty.call(obj, property))
			count++;
	}
	return count;
}

function displayPerson() {
	_persons.forEach(function(pers) {
		console.log(pers);
	})
}


function dynamicSort(property) {
	var sortOrder = 1;

	if(property[0] === "-") {
		sortOrder = -1;
		property = property.substr(1);
	}

	return function (a,b) {
		if(sortOrder == -1){
			return b[property].localeCompare(a[property]);
		} else{
			return a[property].localeCompare(b[property]);
		}        
	}
}


function generate() {
	for(let i = 0; i < _nbPerson; i++) {
		let p = {
			firstName	: faker.name.firstName(),
			lastName 	: faker.name.lastName(),
			city		: faker.address.city(),
			job			: faker.name.jobTitle()
		};
		_persons.push(p);
	}
}