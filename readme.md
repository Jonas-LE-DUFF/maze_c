problème d'optimisation : 
- répétition de la récupération des murs "retirables"
- on ne rapetisse pas la liste de monstres après en avoir éliminé un
- on n'utilise pas de size_t ou de unsigned int alors qu'on pourrait
- la fonction de libération de matrice n'a pas été généralisée
- quelques répétitions ici et là

problèmes logiques : 
- les monstres bougent quasiment aléatoirement
- les orcs continuent de rester autour du même trésor même si celui ci a été collecté
- parfois le code fait des "revérifications" comme pour les générations de labyrinthe avec les  nombres impairs mais par exemple on ne reteste pas pour la taille maximale du labyrinthe
- les monstres sont positionnées de anière purement hasardeuse

usage de l'IA : 
- utilisation partielle pour résoudre des bugs difficiles à comprendre
- aucune utilisation dans la compréhension du sujet
- une seule intervention dans l'écriture du code (minime... 3-4 lignes)
