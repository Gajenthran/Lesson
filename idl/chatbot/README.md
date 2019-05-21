# Chatbot (chauffeur taxi)

Le bot est destiné à remplacer les chauffeur taxi afin de faciliter le déplacement et les démandes du client.

# Usage

Pour lancer une conversation avec le chatbot, il suffit d'exécuter la commande suivante:
```java -cp lib/Ab.jar Main bot=sun=chat trace=false```


# AIML

Pour réaliser notre chatbot, nous avons utiliser AIML (Artificial Intelligence Markup Language). Nous discuterons dans les sections suivantes des différentes stratégies employées.

## Fichiers contexuelles et hors contexte

Nous pouvons séparer les fichiers aiml en deux groupes avec ceux qui ont un contexte et ceux qui ne dépendent pas du contexte. Par exemple le profil, les salutations et les questions concernant le taxi sont dépendants du contexte. Les questions commençant par "c'est", "humour", "qu'est ce que" sont souvent hors contexte, c'est-à-dire qu'ils peuvent être exploités à n'importe quel moment (par exemple qu'est ce qu'un chat).

## Les fichiers autres qu'AIML

Nous stockerons les informations principales concernant le bot se trouvent dans properties.txt (config). Sinon nous aurons un ensemble de modèles dans le dossier sets qui regroupera un ensemble de fichier, tous traitant un thème précis (voiture, famille, animal...). Je tiens à préciser que pour les questions "bâteau", je me suis aidé de données se trouvant sur Internet (comme Alicebot AIML) afin de ne pas perdre trop de temps.
De plus les fichiers ayant le suffixe ```_srai```permettent de regrouper un ensemble de questions portant sur un même sujet au lieu d'écrire plusieurs fois la même réponse, nous allons le rediriger vers une seule et même catégorie afin d'avoir un code plus lisible.

## La balise SRAI

La balise a été de nombreuses fois exploité afin d'anticiper les abréviations et les fautes potentielles que pouvaient faire l'utilisateur. En effet, la balise ```<SRAI>``` permet d'afficher le même résultat pour le même modèle, ainsi lorsque nous écrivons C EST QUOI et C QUOI, si C QUOI faisons une référence explicite au C EST QUOI à l'aide de ```SRAI```
nous aurons donc le même résultat que le pattern C EST QUOI. Cette balise est extrêment utile et efficace pour éviter les redondances et les fautes dû à une mauvaise orthographe.

## That

La balise ```that``` nous a également servi à enchaîner des conversations afin de créer une certaines discussions entre le client et le chauffeur. Très simple et très efficace à utiliser. A exploiter davantage dans notre code !

## Random

```random``` nous a permis d'éviter une discussion trop répétives avec les mêmes réponses à chaque question. Par exemple, pour les blagues, nous allons utiliser un random pour éviter de raconter la même blague.

## Topic

```topic``` aussi a été primordial dans notre fichier, surtout que mon bot est dédié à un but précis à savoir celui de chauffeur taxi donc forcément il fallait créer des topic pour bien distinguer les questions hors contexte et contextuelles.

## Get, Set

Les ```get``` et ```set``` permettent d'obtenir un discours dans la durée, notamment grâce au fait qu'on puisse retenir les informations dans des ```set``` et récupérer avec ```get```. Cela nous permet de sauvegarder des données primordiales comme le nom du client qui peut être réutiliser tout au long de la conversation.