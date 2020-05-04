// 1)
val rdd = sc.parallelize(Seq(("Gajenthran", "France", 21), ("Loic", "Cote d'Ivoire", 17), ("Kaxandra", "France", 20), ("Erkan", "France", 22), ("Amin", "Iran", 25)));

// 2)
rdd.foreach(println);

/* res:
(Gajenthran,France,21)
(Kaxandra,France,20)
(Loic,Cote d'Ivoire,17)
(Erkan,France,22)
(Amin,Iran,25)
*/

// 3)
val df = rdd.toDF("name", "loc", "age");
df.groupBy($"loc").agg(avg($"age"))

/* res: res4: org.apache.spark.sql.DataFrame = [loc: string, avg(age): double] */

// 4)
val df = rdd.toDF("name", "loc", "age");

// 5)
df.printSchema()

/* res:
root
 |-- name: string (nullable = true)
 |-- loc: string (nullable = true)
 |-- age: integer (nullable = false)
*/

// 6)
df.show()

/* res:
+----------+-------------+---+
|      name|          loc|age|
+----------+-------------+---+
|Gajenthran|       France| 21|
|      Loic|Cote d'Ivoire| 17|
|  Kaxandra|       France| 20|
|     Erkan|       France| 22|
|      Amin|         Iran| 25|
+----------+-------------+---+
*/

// 7) (censé être ville mais pas envie)
case class Personne(nom: String, pays: String, age: Int)

// 8) 
val persRDD = sc.parallelize(Seq(
	Personne("Gajenthran", "France", 21), 
	Personne("Loic", "Cote d'Ivoire", 17), 
	Personne("Kaxandra", "France", 20), 
	Personne("Erkan", "France", 22), 
	Personne("Amin", "Iran", 25)));

// 9)
persRDD.foreach(println)

/* res:
(Gajenthran,France,21)
(Kaxandra,France,20)
(Loic,Cote d'Ivoire,17)
(Erkan,France,22)
(Amin,Iran,25)
*/

// 10) (pas besoin de map, filter suffit)
// Chercher les personnes mineures
val mineur = persRDD.filter(p => p.age < 18)
mineur.foreach(println)

/* res: Personne(Loic,Cote d'Ivoire,17) */

// 11)
val persDF = persRDD.toDF("nom", "pays", "age");

// 12)
persDF.printSchema()

/* res:
root
 |-- nom: string (nullable = true)
 |-- pays: string (nullable = true)
 |-- age: integer (nullable = false)
*/

// 13)
persDF.show()

/* res:
+----------+-------------+---+
|       nom|         pays|age|
+----------+-------------+---+
|Gajenthran|       France| 21|
|      Loic|Cote d'Ivoire| 17|
|  Kaxandra|       France| 20|
|     Erkan|       France| 22|
|      Amin|         Iran| 25|
+----------+-------------+---+
*/

// 14)
val persDS = persRDD.toDS()

// 15)
// Chercher les personnes étrangers
persDF.filter(persDF("pays") =!= "France").show()

/* res:
+----+-------------+---+
| nom|         pays|age|
+----+-------------+---+
|Loic|Cote d'Ivoire| 17|
|Amin|         Iran| 25|
+----+-------------+---+
*/

// 16)
val selectNom = persDF.select("nom")
selectNom.printSchema()
selectNom.show()

/* res:
root
 |-- nom: string (nullable = true)

+----------+
|       nom|
+----------+
|Gajenthran|
|      Loic|
|  Kaxandra|
|     Erkan|
|      Amin|
+----------+
*/

// 17) (Alias du filter)
// Chercher les personnes mineures
persDF.where($"age" < 18).show()

/* res:
+----+-------------+---+
| nom|         pays|age|
+----+-------------+---+
|Loic|Cote d'Ivoire| 17|
+----+-------------+---+
*/

// 18) (Problème avec la version de Java)
persDF.select(count($"nom")).show()
persDF.select(count($"nom").alias("nb_personnes")).show()

/* res:
+----------+
|count(nom)|
+----------+
|         5|
+----------+
*/

// 19)
persDF.groupBy($"pays").agg(count($"nom").alias("nb_personnes"))

/* res:
+-------------+------------+
|         pays|nb personnes|
+-------------+------------+
|       France|           3|
|         Iran|           1|
|Cote d'Ivoire|           1|
+-------------+------------+
*/

// 20)
persDS.groupBy($"pays").agg(count($"nom").alias("nb_personnes"), avg($"age").alias("age moyen")).show()

/* res:
+-------------+------------+---------+
|         pays|nb_personnes|age moyen|
+-------------+------------+---------+
|       France|           3|     21.0|
|         Iran|           1|     25.0|
|Cote d'Ivoire|           1|     17.0|
+-------------+------------+---------+
*/

persDS.groupBy($"pays").agg(count($"nom").alias("nb_personnes"), min($"age").alias("age min")).show()

/* res:
+-------------+------------+-------+
|         pays|nb_personnes|age min|
+-------------+------------+-------+
|       France|           3|     20|
|         Iran|           1|     25|
|Cote d'Ivoire|           1|     17|
+-------------+------------+-------+
*/

persDS.groupBy($"pays").agg(count($"nom").alias("nb_personnes"), max($"age").alias("age max")).show()

/* res:
+-------------+------------+-------+
|         pays|nb_personnes|age max|
+-------------+------------+-------+
|       France|           3|     22|
|         Iran|           1|     25|
|Cote d'Ivoire|           1|     17|
+-------------+------------+-------+
*/

// 21)
val persRDD = persDF.rdd

// 22)
persDF.createOrReplaceTempView("personne")

// 23)
// Pour ceux qui ont un problème avec Hive, essayez la commande suivante: 
//   spark-shell --conf spark.sql.catalogImpleme=in-memory
spark.sql("SELECT * FROM personne").show()

/* res:
+----------+-------------+---+
|       nom|         pays|age|
+----------+-------------+---+
|Gajenthran|       France| 21|
|      Loic|Cote d'Ivoire| 17|
|  Kaxandra|       France| 20|
|     Erkan|       France| 22|
|      Amin|         Iran| 25|
+----------+-------------+---+
*/

// 24) (Chercher les personnes majeures)
val majeur = spark.sql("SELECT nom, age FROM personne WHERE age > 17")
majeur.show()
majeur.filter($"age" > 20).show(2)

// 25)
val foyersDF = spark.read.json("/Users/gajen/Downloads/spark-sql/Foyers.json")

// 26)
foyersDF.printSchema()

/* res:
root
 |-- nom: string (nullable = true)
 |-- prenom: string (nullable = true)
 |-- taille: long (nullable = true)
 */

 // 27)
 foyersDF.show()

 /* res:
 +---------+--------+------+
|      nom|  prenom|taille|
+---------+--------+------+
| Harrison|  Gerald|     2|
|Roosevelt|    Bill|     3|
|    Tyler|Franklin|     1|
|  Johnson| Woodrow|     5|
| Harrison| Woodrow|     3|
|   Truman| William|     1|
|   Monroe|  Thomas|     2|
|    Grant|Benjamin|     2|
|   Truman|   Harry|     4|
| Harrison|    Bill|     6|
+---------+--------+------+
*/

// 28) (pas compris)

// 29)
foyersDF.filter($"taille" > 2).orderBy(asc("taille")).show()

/* res:
+---------+-------+------+
|      nom| prenom|taille|
+---------+-------+------+
|Roosevelt|   Bill|     3|
| Harrison|Woodrow|     3|
|   Truman|  Harry|     4|
|  Johnson|Woodrow|     5|
| Harrison|   Bill|     6|
+---------+-------+------+
*/

// 30) 
foyersDF.createOrReplaceTempView("foyers")
// (Chercher les foyers dont la taille est != 3 + nom du propriétaire comporte un "o")
spark.sql("SELECT * FROM foyers WHERE taille <> 3 AND nom like %o%").show()

/* res:
+--------+-------+------+
|     nom| prenom|taille|
+--------+-------+------+
|Harrison| Gerald|     2|
| Johnson|Woodrow|     5|
|  Monroe| Thomas|     2|
|Harrison|   Bill|     6|
+--------+-------+------+
*/

// 31-32)
// 1ère étape: DF pour foyers ==> FAIT

// 2ème étape: DF pour employes

// 1ère option: considération du header en csv
// 2ème option: délimitation des valeurs par des points-virgules
// 3ème option: séparation des valeurs par des double quote
val employesDF = spark.read.format("csv").option("header", "true").option("delimiter", ";").option("quote", "").load("/Users/gajen/Downloads/spark-sql/Employes.csv")
employesDF.show()

/* res:
+------+---------+--------+--------+---------+
|numero|      nom|  prenom|     ddn|  adresse|
+------+---------+--------+--------+---------+
|     1| Harrison|  Gerald|20/02/08|    Paris|
|     2|Roosevelt|    Bill|11/07/05|     Lyon|
|     3|    Tyler|Franklin|05/08/94|     Lyon|
|     4|  Johnson| Woodrow|19/06/06|     Lyon|
|     5| Harrison| Woodrow|16/01/89|    Paris|
|     6|   Truman| William|02/10/94|    Paris|
|     7|   Monroe|  Thomas|17/07/06|    Paris|
|     8|    Grant|Benjamin|20/06/97|    Brest|
|     9|   Truman|   Harry|03/07/96|    Brest|
|    10| Harrison|    Bill|23/03/90|Marseille|
+------+---------+--------+--------+---------+
*/


// 3ème étape: Jointure
val foyersEmployesDF = foyersDF.join(employesDF, Seq("nom", "prenom"), "left_outer")
foyersEmployesDF.printSchema()

/* res:
root
 |-- nom: string (nullable = true)
 |-- prenom: string (nullable = true)
 |-- taille: long (nullable = true)
 |-- numero: string (nullable = true)
 |-- ddn: string (nullable = true)
 |-- adresse: string (nullable = true)
 */

// 33)
val grandFoyers = foyersEmployesDF.filter($"taille" > 5)

// 34) (pas réussi...)
val grandFoyersWriter = grandFoyers.write
grandFoyersWriter.json("/Users/gajen/Downloads/spark-sql/grandFoyers.json")
grandFoyersWriter.option("header", "true").csv("/Users/gajen/Downloads/spark-sql/grandFoyers.csv")

// 35)
// nombre de colonnes
employesDF.columns.size

/* res:
res54: Int = 5
*/

// nombre d'enregistrements
employesDF.agg(count($"nom")).show()

/* res:
+----------+
|count(nom)|
+----------+
|        10|
+----------+
*/

// 36)
employesDF.groupBy("adresse").agg(count($"nom").alias("nb_employes")).show()

/* res:
+---------+-----------+
|  adresse|nb_employes|
+---------+-----------+
|Marseille|          1|
|    Paris|          4|
|     Lyon|          3|
|    Brest|          2|
+---------+-----------+
*/

// 37)
employesDF.stat.crosstab("nom", "prenom").show()

/* res:
+----------+--------+----+--------+------+-----+------+-------+-------+
|nom_prenom|Benjamin|Bill|Franklin|Gerald|Harry|Thomas|William|Woodrow|
+----------+--------+----+--------+------+-----+------+-------+-------+
|    Truman|       0|   0|       0|     0|    1|     0|      1|      0|
|     Tyler|       0|   0|       1|     0|    0|     0|      0|      0|
|   Johnson|       0|   0|       0|     0|    0|     0|      0|      1|
|  Harrison|       0|   1|       0|     1|    0|     0|      0|      1|
|     Grant|       1|   0|       0|     0|    0|     0|      0|      0|
|    Monroe|       0|   0|       0|     0|    0|     1|      0|      0|
| Roosevelt|       0|   1|       0|     0|    0|     0|      0|      0|
+----------+--------+----+--------+------+-----+------+-------+-------+
*/

// 38)
employesDF.stat.freqItems(Array("nom", "adresse"), 0.3).show()

/* res: 
+------------------+----------------+
|     nom_freqItems|prenom_freqItems|
+------------------+----------------+
|[Harrison, Truman]|   [Bill, Harry]|
+------------------+----------------+
*/

// 39) 
// 1ère étape: Créer un ensemble (nom, prenom)
val npEmployes = foyersDF.select(struct("nom", "prenom").as("nom-prenom"))

/* res:
+-------------------+
|         nom-prenom|
+-------------------+
| [Harrison, Gerald]|
|  [Roosevelt, Bill]|
|  [Tyler, Franklin]|
| [Johnson, Woodrow]|
|[Harrison, Woodrow]|
|  [Truman, William]|
|   [Monroe, Thomas]|
|  [Grant, Benjamin]|
|    [Truman, Harry]|
|   [Harrison, Bill]|
+-------------------+
*/

// 2ème étape: Calculer la fréquence de présence des éléments avec un seuil donné (0.5)
val freqNp = npEmployes.stat.freqItems(Array("nom-prenom"), 0.5)

/* res:
+--------------------+
|nom-prenom_freqItems|
+--------------------+
|[[Monroe, Thomas]...|
+--------------------+
*/

// 3ème étape: Visualisation
freqNp.select(explode($"nom-prenom_freqItems").as("nom-prenom-freq")).show()

/* res:
+-----------------+
| nom-prenom-freq|
+----------------+
|[Harrison, Bill]|
+----------------+
*/

// 40
val udfUpperCase = udf { s: String => s.toUpperCase }

// 41
employesDF.select(udfUpperCase($"nom")).show(5)

/* res:
+---------+
| UDF(nom)|
+---------+
| HARRISON|
|ROOSEVELT|
|    TYLER|
|  JOHNSON|
| HARRISON|
+---------+
*/

// 42
// 1ère étape: Création d'une fonction scala
def calculerAge(ddn: String): Int = {
	import java.util.Date
	import java.text.SimpleDateFormat;
	val now = new Date();
	val date = new SimpleDateFormat("dd/MM/yyyy").parse(ddn);
	now.getYear() - date.getYear();
}

// 2ème étape: fonction comme étant une UDF
val udfCalculerAge = udf(calculerAge(_ : String))

// 43
employesDF.select($"nom", $"ddn", udfCalculerAge($"ddn").alias("age")).show(5)

/* res:
+---------+--------+----+
|      nom|     ddn| age|
+---------+--------+----+
| Harrison|20/02/08|2012|
|Roosevelt|11/07/05|2015|
|    Tyler|05/08/94|1926|
|  Johnson|19/06/06|2014|
| Harrison|16/01/89|1931|
+---------+--------+----+
*/

// 44 - 45
// Je pense qu'il manque un fichier pcq on a pas le revenu des personnes
// On va calculer du coup le niveau des entreprises (Employeurs.csv)
// Pensez à rajouter une ligne sur le fichier csv pour le header

val employeursDF = spark.read.format("csv").option("header", "true").option("delimiter", ";").option("quote", "").load("/Users/gajen/Downloads/spark-sql/Employeurs.csv")
employeursDF.show()

/* res:
+------+----------+------+
|Numero|Entreprise|Niveau|
+------+----------+------+
|     1|       EDF|3000.0|
|     2|    Orange|2500.0|
|     3|       EDF|5000.0|
|     4|Air France|5000.0|
|     5|       EDF|3500.0|
|     6|    Orange|1500.0|
|     7|    Orange|2000.0|
|     8|       EDF|2500.0|
|     9|       EDF|2200.0|
|    10|Air France|4500.0|
+------+----------+------+
*/

// Calculer le statut de l'entreprise selon le niveau.
// Ces deux exos servent juste à savoir utiliser les conditions et les udf.
def calculerStatut(niveau: Float): String = {
	if(niveau > 4000) {
		"Bon"
	} else if(niveau < 4000 && niveau > 2000) {
		"Moyen"
	} else {
		"Faible"
	}
}

def udfCalculerStatut = udf(calculerStatut(_ : Float))

employeursDF.select($"Entreprise", $"Niveau", udfCalculerStatut($"Niveau").alias("Statut")).show()

/* res:
+----------+------+------+
|Entreprise|Niveau|Statut|
+----------+------+------+
|       EDF|3000.0| Moyen|
|    Orange|2500.0| Moyen|
|       EDF|5000.0|   Bon|
|Air France|5000.0|   Bon|
|       EDF|3500.0| Moyen|
|    Orange|1500.0|Faible|
|    Orange|2000.0|Faible|
|       EDF|2500.0| Moyen|
|       EDF|2200.0| Moyen|
|Air France|4500.0|   Bon|
+----------+------+------+
*/

// 46) (Pas de manipulation)

// 47) 
spark.udf.register("udfCalculerAge", udfCalculerAge)
spark.udf.register("udfCalculerStatut", udfCalculerStatut)

// 48)
employeursDF.createOrReplaceTempView("employeurs")
spark.sql("SELECT Entreprise, Niveau, udfCalculerStatut(Niveau) as Status FROM employeurs").show()

/* res:
+----------+------+------+
|Entreprise|Niveau|Status|
+----------+------+------+
|       EDF|3000.0| Moyen|
|    Orange|2500.0| Moyen|
|       EDF|5000.0|   Bon|
|Air France|5000.0|   Bon|
|       EDF|3500.0| Moyen|
|    Orange|1500.0|Faible|
|    Orange|2000.0|Faible|
|       EDF|2500.0| Moyen|
|       EDF|2200.0| Moyen|
|Air France|4500.0|   Bon|
+----------+------+------+
*/
