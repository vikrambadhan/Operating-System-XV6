#include "types.h"
#include "stat.h"
#include "user.h"


char buf[10000];
char previous[10000];
char current[10000];


unsigned char toLower(unsigned char ch) {
    if (ch >= 'A' && ch <= 'Z')
        ch = 'a' + (ch - 'A');
    return ch;
 }

int strcasecmp(const char *s1, const char *s2) {
    const unsigned char *us1 = (const unsigned char *)s1;
    const unsigned char *us2 = (const unsigned char *)s2;

    while (toLower(*us1) == toLower(*us2++))
        if (*us1++ == '\0')
            return (0);
    return (toLower(*us1) - toLower(*--us2));
}

void uniq(int fd, char *name, int count, int duplicate, int ignore){
    int i, n;
    int index = 0;
    int cnt = 1;
    previous[0] = '\0';
    // Read through the file and store the values in buffer
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (i = 0; i <= n; i++) {      
                //Perform the operations only if the new line character is encountered. 
            if (buf[i] == '\n' || buf[i] == '\0')
            {
                // If the ignore flag is set, compare the lines while ignoring the case.
                if((ignore == 0 && strcmp(previous,current) == 0) || (ignore==1 && strcasecmp(previous,current) == 0)){
                    cnt++;
                    // If the duplicate flag is set, print only the duplicate lines
                    if(duplicate == 1 && cnt == 2){
                        printf(1,"%s\n",previous);   
                    }
                }else{                                        
                    if(duplicate == 0 && previous[0] != '\0')
                    {
                        if(count == 1){
                            printf(1, "%d ",cnt);    
                        }
                        printf(1,"%s\n",previous);                        
                    }
                    current[index] = '\0';
                    // Copy the current line to previous line. 
                    strcpy(previous, current);
                    previous[index] = '\0';                    
                    cnt = 1;
                }
                index = 0;
            }
            else{
                // Keep adding the characters to current array unless a new line character is encountered. 
                current[index] = buf[i];
                index++;
            }
        }
    }
    if(duplicate == 0){
        if(count == 1){
            printf(1, "%d ",cnt);    
        }
        current[index] = '\0';
        printf(1,"%s\n",previous);        
        if (n < 0) {
            printf(1, "wc: read error\n");
            exit();
        }
    }

}

int main(int argc, char *argv[]){
int fd, i;
int count =0, duplicate=0,ignore = 0;

  if (argc <= 1) {
    uniq(0, "",count,duplicate,ignore);
    exit();
  }

  for (i = 1; i < argc; i++) {
    if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i],"-C") == 0){
        count = 1;
    }
    else if(strcmp(argv[i],"-d") == 0 || strcmp(argv[i],"-D") == 0){
        duplicate = 1;
    }
    else if(strcmp(argv[i],"-i") == 0 || strcmp(argv[i],"-I") == 0){
        ignore = 1;
    }
    else{
        if ((fd = open(argv[i], 0)) < 0) {
        printf(1, "uniq: cannot open %s\n", argv[i]);
        exit();
        }
        
    }
    

  }
  uniq(fd, argv[i],count,duplicate,ignore);
  close(fd);
  exit();
}