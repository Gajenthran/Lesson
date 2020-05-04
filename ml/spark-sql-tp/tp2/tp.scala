// PANCHALINGAMOORTHY Gajenthran
// LOKO Loic
// SALEHI MOTAMED Amin

/* Exercice 1 */
// 1
case class Comm(id: String, date: String, refProduit: String, quantite: Int, adresse: String)

val commRDD = sc.parallelize(Seq(
	Comm("c1","12/03/2019","voiture",3,"Paris, France"),
	Comm("c2","22/02/2018","moto",5,"Paris, France"),
	Comm("c3","08/05/2019","moto",2,"Berlin, Allemagne"),
	Comm("c4","30/01/2018","voiture",10,"Paris, France"),
	Comm("c5","30/01/2018","velo",4,"Berlin, Allemagne"),
	Comm("c6","08/05/2019","voiture",3,"Londres, Angleterre"),
	Comm("c7","08/05/2019","voiture",1,"Berlin, Allemagne"),
	Comm("c8","24/07/2019","velo",6,"Paris, France")))

val commDF = commRDD.toDF("id", "date", "refProduit", "quantite", "adresse");

commDF.show()
/* res: 
+---+----------+----------+--------+-------------------+
| id|      date|refProduit|quantite|            adresse|
+---+----------+----------+--------+-------------------+
| c1|12/03/2019|   voiture|       3|      Paris, France|
| c2|22/02/2018|      moto|       5|      Paris, France|
| c3|08/05/2019|      moto|       2|  Berlin, Allemagne|
| c4|30/01/2018|   voiture|      10|      Paris, France|
| c5|30/01/2018|      velo|       4|  Berlin, Allemagne|
| c6|08/05/2019|   voiture|       3|Londres, Angleterre|
| c7|08/05/2019|   voiture|       1|  Berlin, Allemagne|
| c8|24/07/2019|      velo|       6|      Paris, France|
+---+----------+----------+--------+-------------------+
*/

// 1
commDF.agg(count($"id").alias("nb_commandes")).show()

/* res:
+------------+
|nb_commandes|
+------------+
|           8|
+------------+
*/

// 2
commDF.filter("quantite < 6 AND adresse = 'Paris, France'").show()
// Ou en utilisant UPPER si on reprend la base de données initiale
// commDF.filter("quantite < 6 AND UPPER(adresse) = UPPER('Paris, France')").show()

/* res:
+---+----------+----------+--------+-------------+
| id|      date|refProduit|quantite|      adresse|
+---+----------+----------+--------+-------------+
| c1|12/03/2019|   voiture|       3|Paris, France|
| c2|22/02/2018|      moto|       5|Paris, France|
+---+----------+----------+--------+-------------+
*/

// 3
commDF.groupBy("adresse").agg(sum($"quantite").alias("nb_commandes")).show()
/* res:
+-------------------+------------+
|            adresse|nb_commandes|
+-------------------+------------+
|Londres, Angleterre|           3|
|  Berlin, Allemagne|           7|
|      Paris, France|          24|
+-------------------+------------+
*/

// 4
commDF.agg(max($"quantite").alias("qte_max")).show()
/* res: 
+-------+
|qte_max|
+-------+
|     10|
+-------+
*/


// 5
commDF.groupBy($"refProduit").agg(avg($"quantite")).show()

/* res: 
+----------+-------------+
|refProduit|avg(quantite)|
+----------+-------------+
|      velo|          5.0|
|   voiture|         4.25|
|      moto|          3.5|
+----------+-------------+
*/


// 6
val qtePays = commDF.groupBy("adresse").agg(sum($"quantite").alias("nb_commandes"));
qtePays.where($"nb_commandes" < 2).show()
// Aucun pays n'a moins de commande comme on peut le voir sur la table.
/* res:
+-------+------------+
|adresse|nb_commandes|
+-------+------------+
+-------+------------+
*/

// 7
val qtePays = commDF.groupBy("adresse").agg(sum($"quantite").alias("nb_commandes"));
// qtePays.select("adresse", "nb_commandes").agg(max($"nb_commandes")).show();
/* res:
+-------------------+------------+
|            adresse|nb_commandes|
+-------------------+------------+
|Londres, Angleterre|           3|
|  Berlin, Allemagne|           7|
|      Paris, France|          24|
+-------------------+------------+
*/


// 8
// J'ai eu un problème avec spark.sql pour concaténer, donc j'admets que je concatène
// avec sql pour pouvoir continuer l'exercice 

/* 
case class Ville(ville: String, pays: String, distance: Int)
val villesRDD = sc.parallelize(Seq(
	Ville("Rome", "Italie", 150),
	Ville("Paris", "France", 200),
	Ville("Londres", "Angleterre", 150),
	Ville("Berlin", "Allemagne", 300)))

val villesDF = villesRDD.toDF("ville", "pays", "distance")
villesDF.createOrReplaceTempView("villes")
val vDF = spark.sql("select ville || ', ' ||pays, distance as adresse from villes");

*/

case class Ville(adresse: String, distance: Int)

 val villesRDD = sc.parallelize(Seq(
	Ville("Rome, Italie", 150),
	Ville("Paris, France", 200),
	Ville("Londres, Angleterre", 150),
	Ville("Berlin, Allemagne", 300)))

val villesDF = villesRDD.toDF("adresse_v", "distance")

val villeCommDF = villesDF.join(commDF, $"adresse" === $"adresse_v", "inner");

/* res:
+-------------------+--------+---+----------+----------+--------+-------------------+
|          adresse_v|distance| id|      date|refProduit|quantite|            adresse|
+-------------------+--------+---+----------+----------+--------+-------------------+
|Londres, Angleterre|     150| c6|08/05/2019|   voiture|       3|Londres, Angleterre|
|  Berlin, Allemagne|     300| c3|08/05/2019|      moto|       2|  Berlin, Allemagne|
|  Berlin, Allemagne|     300| c5|30/01/2018|      velo|       4|  Berlin, Allemagne|
|  Berlin, Allemagne|     300| c7|08/05/2019|   voiture|       1|  Berlin, Allemagne|
|      Paris, France|     200| c1|12/03/2019|   voiture|       3|      Paris, France|
|      Paris, France|     200| c2|22/02/2018|      moto|       5|      Paris, France|
|      Paris, France|     200| c4|30/01/2018|   voiture|      10|      Paris, France|
|      Paris, France|     200| c8|24/07/2019|      velo|       6|      Paris, France|
+-------------------+--------+---+----------+----------+--------+-------------------+
*/



/* Exercice 2 */

// 1
val persDF = spark.read.format("csv").option("header", "true").option("delimiter", ";").option("quote", "").load("/Users/gajen/Downloads/tp-spark/Personnels.csv")
persDF.show()

def calculerAge(ddn: String): Int = {
	import java.util.Date
	import java.text.SimpleDateFormat;
	if(ddn == null)
		0
	val now = new Date();
	val date = new SimpleDateFormat("dd/MM/yyyy").parse(ddn);
	now.getYear() - date.getYear();
}

val udfCalculerAge = udf(calculerAge(_ : String))

def verifierNom(nom: String): Int = {
	if(nom == null) {
		0
	} else if(nom.length == 1) {
		0
	} else
		1
}

val udfVerifierNom = udf(verifierNom(_ : String))

val perDF = persDF.filter("DDN is not null");
val persDF = persDF.filter("Nom is not null");
val persDF = perDF.select($"Numero", $"Nom", $"Prenom", $"VillePays", udfCalculerAge($"DDN").alias("Age"))
val perDF = persDF.filter($"Age" > 17)

// pas fait les noms mal renseignés
/* res:
+------+---------+--------+------------+---+
|Numero|      Nom|  Prenom|   VillePays|Age|
+------+---------+--------+------------+---+
|     1| Harrison|  Gerald|  Paris - FR| 23|
|     2|Roosevelt|    Bill|  PARIS - fr| 20|
|     3|    Tyler|Franklin|Londres - UK| 26|
|     4|  Johnson| Woodrow|     LONdres| 44|
|     5|    hatem| Woodrow|    New York| 31|
|     6|   Truman| William|  PARis - fr| 26|
|     8|    Grant|Benjamin|   Lyon - Fr| 23|
|     9|   Truman|   Harry|   lyon - fR| 24|
|    10| Harrison|    Bill|     londres| 30|
|    14|     3Lem|      Wo|    New York| 30|
+------+---------+--------+------------+---+
*/

