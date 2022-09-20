extern int disc_count,wx,wy,game_ready,game_start,save_valid;


int checkSave(FILE* save){
	// Turns aren't checked, too much of a headache, too little time
	int t, mt;
	fscanf(save,"%d/%d",&t,&mt);
	if(mt<t || mt<0 || t<0){
		return 1;									// Invalid turn count
	}
	char ch;
	fscanf(save,"%c",&ch);
	int dc = ch-'A';
	int u[dc];
	for(int i=0;i<dc;i++){
		u[i]=0;
	}
	int d=0,p;
	for(int i=0;i<dc;i++){
		fscanf(save,"%c",&ch);
		p=d;
		d = ch-'A';
		if(ch!='/'){
			if(d>=dc){
				return 2;							// Invalid Disc size
			}
			if(!u[d]){
				u[d]++;
			}else{
				return 3;							// Duplicate Discs
			}
			if(p>d){
				return 4;							// Illegal configuration
			}
		}else{
			p=0;
			i--;
			continue;
		}
	}
	return 0;
}


void resetGame(node** peg, int** turn_seq, int* turns, int* max_turns){
	// Resetting the pegs to the starting state...
	for(int i=0;i<3;i++){									// Free up all the pegs
		while(peg[i]!=NULL){
			node* ptr;
			ptr = peg[i];
			peg[i] = peg[i]->next;							// Done element by element to avoid memory leaks
			free(ptr);
		}
	}
	for(int i=disc_count;i>0;i--){
		int temp = i;
		push(&temp,&(peg[0]));
	}
	// Emptying the move array...
	for(int i = *turns;i>0;i--){
		free(turn_seq[i]);
	}
	turn_seq[0] = (int*)malloc(sizeof(int)*2);
	// Setting move count to 0
	*turns = 0;
	*max_turns = 0;
}

void saveGame(node** peg, int** turn_seq, int* turns, int* max_turns){
	// Picked a specific name for the savefile, I don't wanna deal with OS-specific ways for checking a directory
	FILE* save = fopen("ToH_save.txt", "w+");

	// Save the disc count and discs on pegs
	// Use letters instead of numbers to not have problems when there are more than 10 discs
	fprintf(save,"%d/%d",*turns,*max_turns);
	fprintf(save,"%c",(disc_count+'A'));
	for(int i=0;i<3;i++){
		node* temp = peg[i];
		while(temp!=NULL){
			fprintf(save,"%c",temp->num + 'A'-1);
			temp = temp->next;
		}
		fprintf(save,"/");
	}
	fprintf(save,":");
	// Save the turn order as well
	for(int i=0;i<*max_turns;i++){
		fprintf(save,"%d%d",turn_seq[i][0],turn_seq[i][1]);
	}

	fclose(save);
}

char* loadGame(node** peg, int* turns, int* max_turns){
	FILE* save = fopen("ToH_save.txt","r");
	char ch;
	// Load turn and disc counts
	fscanf(save,"%d/%d%c",turns,max_turns,&ch);
	disc_count = ch-'A';

	// Load turns, the order of discs on the pegs are saved backwards
	int buffer[disc_count];
	for(int i=0, j=0, index=0;i<=disc_count;i++){
		fscanf(save,"%c",&ch);
		if(ch=='/'){
			i--;
			for(j--;j>=0;j--){
				push(&(buffer[j]),&peg[index]);
			}
			index++;
			j=0;
			continue;
		}else{
			buffer[j] = (ch-'A'+1);
			j++;
		}
	}

	// Send a buffer with the played moves
	char* s = (char*)malloc(sizeof(char)*(*max_turns*2));
	fscanf(save,"%s",s);
	return s;
}

