void biosgets(char *str); // get a string from the user
void biosputs(char *str); // print a string to the screen
void welcome();           // clear the screen and print the welcome message
void help();              // print the help menu
void cls();               // clear the screen
void mem();    // print the size of the conventional memory of the IBM PC
void quit();   // reboot the machine
void exit();   // reboot the machine
void reboot(); // reboot the machine
int cmpe(char *src, char *dst); // utility function to compare strings

////////////////////////////////////////////
void main() {
  welcome();
  while (1) {
    char str[10];
    biosgets(str);
		 if(cmpe(str,"help")==1) help();
	else if (cmpe(str ,"cls")==1) cls(); 
	else if (cmpe(str ,"mem")==1) mem(); 
	else if ((cmpe(str ,"quit")==1)||(strcmp(str ,"exit")==1)||(strcmp(str ,"reboot")==1)) reboot(); 
	else biosputs("Unknown command\n\r"); 
	

    // Write your code here
  }
}
/////////////////////////////////////////////
void biosputs(char *str) {
  int i = 0;
  char holdc;
  while (str[i]) {
    holdc = str[i++];
    _asm {
			mov ah, 0Eh
			mov al, holdc
			mov bx, 7
			int 10h
    }
  }
}
/////////////////////////////////////////////
void biosgets(char*str2)
{
	char holdc;
	int i=0;
	while(1)
	{_asm{ mov ah,1
	int 21h
	mov holdc,AL
		
		
	}//end asm
		if (holdc == '\r') 
		{ 
		break;
		}
		else 
		{
		str2[i++]=holdc;
			
		}
		
		
		
	}
	str2[i]='\0';
	biosputs("\n\r");
	i=0;
	
	
} 

/////////////////////////////////////////////
void welcome() {
  // Write your code here
  biosputs("wlecom to mini shell\n\r");
}
/////////////////////////////////////////////
void help() {
  biosputs("help - print out this list\n\rcls - clear the screen\n\rmem - print the size of the memory the computer has\n\rquit - reboot the machine\n\rexit - same as quit\n\rrevoot - same as exit \n\r");
}
/////////////////////////////////////////////
void cls() {
	_asm{ 
		mov al, 00h 
		mov ah, 0 
		int 10h 
		
		
		}//end asm
  
}
/////////////////////////////////////////////
void mem() {
	
  // Write your code here
  unsigned int memsize;
char buffer[10];
    int i = 0;
    
    _asm {
        INT 12h   
        mov memsize, AX  
		}//end asm code

    
    

    // Convert integer to string (manual itoa)
    if (memsize == 0) {
        buffer[i++] = '0';
    } else {
        while (memsize > 0) {
            buffer[i++] = (memsize % 10) + '0'; 
            memsize /= 10;
        }
    }
    buffer[i] = '\0';
	biosputs(buffer);
	
}
/////////////////////////////////////////////
void quit() {
  // Write your code here
}
/////////////////////////////////////////////
void exit() {
  // Write your code here
}
/////////////////////////////////////////////
void reboot() {
  // Write your code here
}
/////////////////////////////////////////////
int cmpe(char *src, char *dst) {
    int i = 0;
    
    while (src[i] != '\0' && dst[i] != '\0') { 
        if (src[i] != dst[i])
		{
            return 0;
        }
        i++;
    }

    return 1;
}

//////////////////////////////////////////////
