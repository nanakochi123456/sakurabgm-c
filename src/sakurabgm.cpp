
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define	SAKURABGM_FILENOTFOUND	1
#define	SAKURABGM_WRITEERROR	2
#define	SAKURABGM_REGEXERROR	3
#define	SAKURABGM_MEMALLOCERROR	4
#define	SAKURABGM_KEYLENGTH		5
#define	SAKURABGM_PARMERROR		6
#define	SAKURABGM_PARMKEYERROR	7

#define	STRLENGTH	512
#define	TMPSTRLENGTH	100000

const char *errormsg[]={
	"",
	"File not found",
	"File can't write",
	"Regex error",
	"Out of memory",
	"Out of key length",
	"Paramater ignore",
	"Key not found",
};

void	*_stacks[10000];
int		_stack=0;

void	sakurabgm_exit(int);

void	sakurabgm_error(int msg, char *parm) {
	fprintf(stderr, "ERR%d %s: %s\n", msg+500, errormsg[msg], parm);
	sakurabgm_exit(1);
}

void	*memalloc(size_t size) {
	void	*buf;

	if((buf = (char *)malloc((int)size + 1)) == NULL)
		sakurabgm_error(SAKURABGM_MEMALLOCERROR, (char *)"memalloc");
	_stacks[_stack]=buf;
	_stack++;
	return buf;
}

void	memfree(void *ptr) {
	int		i;
	for(i = 0; i < _stack; i++) {
		if(_stacks[i] == ptr) {
			free(ptr);
			_stacks[i]=NULL;
		}
	}
}

void	banner() {
	fprintf(stderr, "\
sakurabgm version 1.0\n\
Copyright 2015 by nano\n\
");
}

void	sakurabgm_exit(int stat) {
	int	i;

	for(i = 0; i < _stack; i++) {
		if(_stacks[i] != NULL) {
			free(_stacks[i]);
		}
	}
	exit(stat);
}

void	help() {
	fprintf(stderr, "\
		   \n\
Usage : \n\
sakurabgm list [input mmlfile]\n\
sakurabgm replace [input mmlfile] [output mmlfile] ....=.. ....=..\n\
");
}

#define HASHSIZE	4096
#define HASHKEYSIZE	256
typedef struct sbgmlist {
	char		_keyword[HASHKEYSIZE];
	char		_comment[STRLENGTH];
	int			_value;
	int			_minvalue;
	char		_mindesc[STRLENGTH];
	int			_default;
	char		_defdesc[STRLENGTH];
	int			_maxvalue;
	char		_maxdesc[STRLENGTH];
} sbgmlist;
sbgmlist	*sbgmhash[HASHSIZE];

sbgmlist *searchsbgmhash(char *keyword) {
	sbgmlist	*p;
	int		i;
	for(i = 0; sbgmhash[i] != NULL && i < HASHSIZE; i++) {
		if(!strcmp(keyword, sbgmhash[i]->_keyword))
			return sbgmhash[i];
	}
	return NULL;
}

int	addsbgmhash(char *keyword, char *comment, int value, int minvalue, char *mindesc, int def, char *defdesc, int maxvalue, char *maxdesc) {
	sbgmlist	*p;
	int		i;
	p=searchsbgmhash(keyword);
	if(p == NULL) {
		for(i = 0; sbgmhash[i] != NULL && i < HASHSIZE; i++);
		p=(sbgmlist *)memalloc(sizeof(sbgmlist) + 1);
		sbgmhash[i]=p;
	}
	strcpy(p->_keyword, keyword);
	strcpy(p->_comment, comment);
	p->_value=value;
	p->_minvalue=minvalue;
	strcpy(p->_mindesc, mindesc);
	p->_default=def;
	strcpy(p->_defdesc, defdesc);
	p->_maxvalue=maxvalue;
	strcpy(p->_maxdesc, maxdesc);
}

char *readmml(char *file) {
	char	*buf;
	FILE	*fp;
	long	size;
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buf = (char *)memalloc((int)size * 2);
	fread(buf, 1, size, fp);
	fclose(fp);
	return buf;
}

char	*tokens_mml[]={
	(char *)"Int ([[:alnum:]]+)=([[:digit:]]+).*\\/\\/sakurabgm:",
	(char *)"Tempo ([[:digit:]][[:digit:]]+).*\\/\\/sakurabgm:",
	(char *)"Key \\(([[:digit:]]+)\\).*\\/\\/sakurabgm:",
	NULL
};

/*
char	*tokens_sys[]= {
	(char *)"Str NAME={\".*\"}",
	(char *)"Str AUTHOR={\".*\"}",
	(char *)"Str TIMIDITYCFG={\".*\"}",
	(char *)"Str COMMENT={\".*\"}",
	(char *)"Str LICENSE={\".*\"}",
	(char *)"Str GENRE={\".*\"}",
	(char *)"Str URL={\".*\"}",
	(char *)"Str LICENSEURL={\".*\"}",
	(char *)"Str SAKURAURL={\".*\"}",
	(char *)"Str CHORD={\".*\"}",
	NULL
};
*/
char	_tmp[TMPSTRLENGTH];

char	*sakurabgmparm(char *keyword, char *str, int desc) {
	char	*s, *t, *k;

	if((s=strstr(str, (char *)"//sakurabgm:")) == NULL)
		return NULL;
	// キーワードの説明
	if((t=strstr(s, keyword)) == NULL) {
		strcpy(_tmp, s);
		*strstr(_tmp, ",")='\0';
		if(strlen(_tmp) > STRLENGTH) {
			sakurabgm_error(SAKURABGM_KEYLENGTH, _tmp);
		}
		return _tmp;
	}
	// それ以外
	k=t;
	if((k=strstr(k, "=")) != NULL) {
		t=k;
		t++;
	}

	if(desc) {
		k=t;
		if((k=strstr(k, "=")) != NULL) {
			t=k;
			t++;
		}
	}
	strcpy(_tmp, t);

	if((t=strstr(_tmp, ",")) != NULL) {
		*t='\0';
	}
	if((t=strstr(_tmp, "=")) != NULL) {
		*t='\0';
	}
	if((t=strstr(_tmp, "\r")) != NULL) {
		*t='\0';
	}
	if((t=strstr(_tmp, "\n")) != NULL) {
		*t='\0';
	}
	return _tmp;
}

char	*sakurabgm_list(char *MML) {
	char	*tok;
	int		i, j, k, rc;
	regex_t preg;
	regmatch_t	pmatch[3];
	char	*t, *s;
	char	ss[STRLENGTH];
	char	kk[STRLENGTH];
	int		_value, _minvalue, _default, _maxvalue;
	char	_comment[STRLENGTH];
	char	_mindesc[STRLENGTH], _defdesc[STRLENGTH], _maxdesc[STRLENGTH];

	tok=strtok(MML, "\n");
	while(tok != NULL) {
		for(i=0; tokens_mml[i] != NULL; i++) {
			if(regcomp(&preg, tokens_mml[i], REG_EXTENDED | REG_NEWLINE ) != 0 ) {
				sakurabgm_error(SAKURABGM_REGEXERROR, tokens_mml[i]);
			}
			if((rc=regexec( &preg, tok, 3, pmatch, 0 )) != REG_NOMATCH) {
				for(j=0; j<3; j++ ) {
					if((t=strstr(tok, "Int ")) != NULL) {
						// キーの取得
						for(k=0; k<4; k++)
							t++;
						for(s=kk; *t != '='; t++, s++) {
							*s=*t;
						}
						*s='\0';
						*t++;
						// 値の取得
						for(s=ss; *t>='0' && *t <= '9'; t++, s++) {
							*s=*t;
						}
						*s='\0';
						_value=atoi(ss);
						_minvalue=atoi(sakurabgmparm((char *)"MIN", tok, 0));
						strcpy(_mindesc, sakurabgmparm((char *)"MIN", tok, 1));
						_default=atoi(sakurabgmparm((char *)"DEFAULT", tok, 0));
						strcpy(_defdesc, sakurabgmparm((char *)"DEFAULT", tok, 1));
						_maxvalue=atoi(sakurabgmparm((char *)"MAX", tok, 0));
						strcpy(_maxdesc, sakurabgmparm((char *)"MAX", tok, 1));
						strcpy(_comment, sakurabgmparm((char *)"COMMENT", tok, 0));

						addsbgmhash(kk, _comment, _value, _minvalue, _mindesc, _default, _defdesc, _maxvalue, _maxdesc);
					}
					if((t=strstr(tok, "Tempo ")) != NULL) {
						// 値の取得
						for(k=0; k<6; k++)
							t++;
						for(s=ss; *t>='0' && *t <= '9'; t++, s++) {
							*s=*t;
						}
						*s='\0';
						_value=atoi(ss);
						_minvalue=atoi(sakurabgmparm((char *)"MIN", tok, 0));
						strcpy(_mindesc, sakurabgmparm((char *)"MIN", tok, 1));
						_default=atoi(sakurabgmparm((char *)"DEFAULT", tok, 0));
						strcpy(_defdesc, sakurabgmparm((char *)"DEFAULT", tok, 1));
						_maxvalue=atoi(sakurabgmparm((char *)"MAX", tok, 0));
						strcpy(_maxdesc, sakurabgmparm((char *)"MAX", tok, 1));
						strcpy(_comment, sakurabgmparm((char *)"COMMENT", tok, 0));

						addsbgmhash((char *)"Tempo", _comment, _value, _minvalue, _mindesc, _default, _defdesc, _maxvalue, _maxdesc);
					}
					if((t=strstr(tok, "Key (")) != NULL) {
						// 値の取得
						for(k=0; k<6; k++)
							t++;
						for(s=ss; *t>='0' && *t <= '9'; t++, s++) {
							*s=*t;
						}
						*s='\0';
						_value=atoi(ss);
						_minvalue=atoi(sakurabgmparm((char *)"MIN", tok, 0));
						strcpy(_mindesc, sakurabgmparm((char *)"MIN", tok, 1));
						_default=atoi(sakurabgmparm((char *)"DEFAULT", tok, 0));
						strcpy(_defdesc, sakurabgmparm((char *)"DEFAULT", tok, 1));
						_maxvalue=atoi(sakurabgmparm((char *)"MAX", tok, 0));
						strcpy(_maxdesc, sakurabgmparm((char *)"MAX", tok, 1));
						strcpy(_comment, sakurabgmparm((char *)"COMMENT", tok, 0));

						addsbgmhash((char *)"Key", _comment, _value, _minvalue, _mindesc, _default, _defdesc, _maxvalue, _maxdesc);
					}
				}
			}
			regfree(&preg);
		}
		tok=strtok(NULL, "\n");
	}

}

int strrep(char *buf, char *before, char *after)
{
    char *tmp;
    size_t beforelen, afterlen;
    
    beforelen = strlen(before);
    afterlen = strlen(after);
    if (beforelen == 0 || (tmp = strstr(buf, before)) == NULL) return 0;
    memmove(tmp + afterlen, tmp + beforelen, strlen(buf) - (tmp + beforelen - buf ) + 1);
    memcpy(tmp, after, afterlen);
    return 1;
}

void	sakurabgm_replace(char *buf, char *str, char *key, char *value) {
	char	*tok;
	char	tmp1[STRLENGTH];
	char	tmp2[STRLENGTH];
	char	tmp3[STRLENGTH];
	tok=strtok(str, "\n");
	*buf=0;
	while(tok != NULL) {
		if(!strcmp(key, (char *)"Tempo")) {
			sprintf(tmp1, "Tempo %d"
				, searchsbgmhash((char *)"Tempo")->_default);
			sprintf(tmp2, "Tempo %s", value);
		} else if(!strcmp(key, (char *)"Key")) {
			sprintf(tmp1, "Key (%d)"
				, searchsbgmhash((char *)"Key")->_default);
			sprintf(tmp2, "Key (%s)", value);
		} else {
			sbgmlist *p;
			p=searchsbgmhash(key);
			if(p == NULL) {
				sakurabgm_error(SAKURABGM_PARMKEYERROR, key);
			}
			sprintf(tmp1, "Int %s=%d"
				, key, searchsbgmhash(key)->_default);
			sprintf(tmp2, "Int %s=%s", key, value);
		}
		strcpy(tmp3, tok);
		strrep(tmp3, tmp1, tmp2);
		strcat(tmp3, "\n");
		strcat(buf, tmp3);
		tok=strtok(NULL, "\n");
	}
}

int sakurabgm(int argc, char * argv[]) {
	char	*MML;
	if (argc >= 3) {
		if (strcmp(argv[1], "list") == 0) {
			if ((MML = readmml(argv[2])) == NULL) {
				return SAKURABGM_FILENOTFOUND;
			}
			sakurabgm_list(MML);
			sbgmlist	*p;
			int	i;
			for(i = 0; sbgmhash[i] != NULL && i < HASHSIZE; i++) {
				p=sbgmhash[i];
				printf("%s=%d DEF=%d/%s MIN=%d/%s MAX=%d/%s\n", p->_keyword, p->_value, p->_default, p->_defdesc, p->_minvalue, p->_mindesc, p->_maxvalue, p->_maxdesc);	}
		}
		if (strcmp(argv[1], "replace") == 0) {
			if ((MML = readmml(argv[2])) == NULL) {
				return SAKURABGM_FILENOTFOUND;
			}
			FILE *wfp;
			if(strcmp(argv[2], argv[3])==0) {
				return SAKURABGM_WRITEERROR;
			}
			if((wfp=fopen(argv[3], "w"))==NULL) {
				return SAKURABGM_WRITEERROR;
			}
			char	*MMLTEMP;
			MMLTEMP=(char *)memalloc(strlen(MML)*2);
			strcpy(MMLTEMP, MML);
			sakurabgm_list(MMLTEMP);
			sbgmlist	*p;
			int		i;
			*MMLTEMP='\0';
			for(i=4; i<argc; i++) {
				char *s;
				char key[STRLENGTH];
				char value[STRLENGTH];

				s=argv[i];
				if(strstr(s, "=") == NULL) {
					sakurabgm_error(SAKURABGM_PARMERROR, s);
				}
				strcpy(key, s);
				*strstr(key, "=")='\0';
				s = strstr(s, "=");
				s++;
				strcpy(value, s);
				sakurabgm_replace(MMLTEMP, MML, key, value);
				strcpy(MML, MMLTEMP);
			}
		fprintf(wfp, MML);
		fclose(wfp);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int stat;
	banner();
	if (argc < 3) {
		help();
		sakurabgm_exit(0);
	}
	stat = sakurabgm(argc, argv);
	if (stat == SAKURABGM_FILENOTFOUND) {
		sakurabgm_error(SAKURABGM_FILENOTFOUND, argv[2]);
	}
	if (stat == SAKURABGM_WRITEERROR) {
		sakurabgm_error(SAKURABGM_WRITEERROR, argv[3]);
	}
	return 0;
}

