/*****************************************************************/
/********************* running app *******************************/
/*****************************************************************/
build: 
			scons
		
run server:
		 	./test --docroot . --http-address 0.0.0.0 --http-port 9090

run client: (in browser) 
			0.0.0.0:9090

/#################################################################/


/*****************************************************************/
/********************* create new branch *************************/
/*****************************************************************/
git branch nazwaBranchu
git checkout nazwaBranchu
// to żeby przełączyć się na nowy branch
no i coś tam zrób i zakomituj
a potem:
git push origin nazwaBranchu
/#################################################################/

//squashing, łaczenie commitow
git rebase -i HEAD~2   //bierze dwa commity od gory i zmieniamy pick->squash 
						//te ktore chcemy polaczyc