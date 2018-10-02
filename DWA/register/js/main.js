window.onload = main;

var _table = null;
var _persons = new Array();
var _nbPerson = 100;

function main() {
	generate(_nbPerson);
	displayPerson();
}

function createTable() {
	let table = document.createElement("table");
	document.body.appendChild(table);

	for(let i = 0; i < 10; i++) {
		var tr = document.createElement("tr");
		table.appendChild(tr);
	}
}

function displayPerson() {
	_persons.forEach(function(pers) {
		console.log(pers);
	})
}
function generate(nbPerson) {
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