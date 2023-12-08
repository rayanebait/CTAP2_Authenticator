Guillhem Mizrahi
Bait Rayane

Rendu équipe NIST.


Le projet est organisé en 3 parties:
	-Un fichier source Authenticator.c contenant 'parse_request' qui dirige les commandes 
entrantes vers les fonctions MakeCredential, GetAssertion, ListCredentials et Reset.
	-Les fichiers source de MakeCredential, GetAssertion, ListCredentials
et Reset qui éxecutent implémentent les fonctions correspondantes.
	-Les utilitaires qui implémentent la gestion des timers, du périphérique USART,
de la gestion des crédentiers ainsi que du générateur pseudo aléatoire.

Les utilitaires sont indépendants les uns des autres en dehors du générateur pseudo aléatoire 
qui dépend du watchdog timer et du timer0 et de get\_approval qui dépend du timer0 et du timer1.
Les fichiers source des fonctions MakeCredential, GetAssertion, ListCredential et Reset sont 
totalement indépendants entre eux et dépendant des utilitaires.
Le fichier source de Authenticator dépend des fichiers source des quatres fonctions.

Définitions:
	-Les STATUS_ERR sont définies dans le fichier error.h comme des types entiers (enum).
	-Les COMMAND sont définies dans Authenticator.h en types entiers (enum).

ISR:
	-get_approval contient une ISR pour le vecteur TIMER1_COMPA_vect qui permet de 
faire clignoter la led tout les 500ms et d'incrémenter un compteur et de donner une limite de 
temps au clignotement.
	-get_approval contient aussi une ISR pour le vecteur PCINT0_vect qui est enclenchée 
lors de l'appui sur le bouton. A l'activation, les interruptions sont désactivée pour 
PCINT0.
	-weak_prg contient une ISR vide pour le vecteur WDT_vect. 
	-uart contient une ISR pour le vecteur USART_RX_vect qui pousse le caractère reçu
dans le ring buffer.
	-uart contient aussi une ISR pour le vecteur TIMER0_OVF_vect qui incrémente un compteur
lorsque le UART__getchar est choisi non bloquant.

Remarques:
	-Dans la boucle de get_approval, si c'est make_credential qui fait la demande 
sleep_mode n'est pas lancé volontairement pour que le temps requis pour appuyer sur le bouton
serve de base aléatoire.

Problèmes rencontrés:
	-On a rencontré quelque problèmes pour trouver une source aléatoire et la faire marcher
au début. On s'est finalement reposé si le fait que le timer0 et le watchdog timer n'ont pas 
le même niveau de précision même si les seed obtenues ne semblent pas si aléatoires.
	-On a aussi rencontrés plusieurs problèmes pour tout ce qui concernait l'eeprom. 
Notamment le fait que l'eeprom est écrite de manière asynchrone et ne dépend pas vraiment 
des interruptions. 
	-Aussi la documentation de avr/eeprom.h dans avr libc décrit eeprom_read/write_byte 
comme prenant en entrée une adresse sur 8 bits (uint8_t \*) ainsi qu'une valeur en cas 
d'écriture. Cependant l'eeprom de l'atmega328p fait 1ko et donc doit être adressée sur 10bits.
Etant peu à l'aise avec le C on ne savait pas si transmettre un (void \*) ou un (uint16_t \*)
permettrait d'accéder à toute la mémoire. 

Pour tester les différentes fonctionnalités, on a implémenté des petits scripts python
simulant le fonctionnement de yubino ou plus simplement modifié STATUS_OK=0 en STATUS_OK=48
pour pouvoir envoyer des entrées valides via screen. 

L'intégralité de la compilation se fait via la commande make. Pour compiler et flasher la carte
on peut enter la commande make flash. Pour compiler un autre binaire disons nommé prog et 
contenu dans un fichier temp. Il faut entrer make flash PROG=temp/prog.


