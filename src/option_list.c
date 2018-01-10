#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

// refer cfg file form
// change from cfg to list
// 추후 해당 option에 맞게 layer 생성
// #: comment in cfg file
// ;: end of string in cfg file
list *
read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    if(file == 0) file_error(filename);
    char *line;
    int nu = 0;
    list *options = make_list();
    while((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                if(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    return options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    if(!name_list) name_list = option_find_str(options, "labels", 0);
    if(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } else {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    return m;
}

//list insert key:value
// e.g. batch=128
// ----------------------
// | key: batch(char)   | -> list
// | value: 128(char)   |
// ----------------------
// batch=128 -> batch'\0'128
//              ^        ^
//              |        |
//              s -> key val
// memory leak?
int 
read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    for(i = 0; i < len; ++i){
        if(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    if(i == len-1) return 0;
    char *key = s;
    option_insert(options, key, val);
    return 1;
}

void 
option_insert(list *l, char *key, char *val)
{
    kvp *p = malloc(sizeof(kvp));
    p->key = key;
    p->val = val;
    p->used = 0;
    list_insert(l, p);
}

void 
option_unused(list *l)
{
    node *n = l->front;
    while(n){
        kvp *p = (kvp *)n->val;
        if(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

// list 내의 특정 key를 가지는 component의 value를 반환한다
char *
option_find(list *l, char *key)
{
    node *n = l->front;
    while(n){
        kvp *p = (kvp *)n->val;
        if(strcmp(p->key, key) == 0){
            p->used = 1;
            return p->val;
        }
        n = n->next;
    }
    return 0;
}

// list내의 key의 value를 찾는다. 찾지 못했을 때는 def를 반환한다?
// 문자열로 반환
char *
option_find_str(list *l, char *key, char *def)
{
    char *v = option_find(l, key);
    if(v) return v;
    if(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    return def;
}

// list내의 key의 value를  찾는다. 찾지 못했을 때는 def로 반환한다.
// int로 반환
int 
option_find_int(list *l, char *key, int def)
{
    char *v = option_find(l, key);
    if(v) return atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    return def;
}

// list내의 key의 value를  찾는다. 찾지 못했을 때는 def로 반환한다.
// float로 반환
float 
option_find_float(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    if(v) return atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    return def;
}

// list내의 key의 value를  찾는다. 찾지 못했을 때는 def로 반환한다.
// 정수로 반환
// 경고문 없음
int 
option_find_int_quiet(list *l, char *key, int def)
{
    char *v = option_find(l, key);
    if(v) return atoi(v);
    return def;
}

// list내의 key의 value를  찾는다. 찾지 못했을 때는 def로 반환한다.
// 실수로 반환
// 경고문 없음
float 
option_find_float_quiet(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    if(v) return atof(v);
    return def;
}
