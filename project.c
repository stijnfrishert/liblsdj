#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

void lsdj_write_lsdsng(const lsdj_project_t* project, const char* path, lsdj_error_t** error)
{
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return lsdj_create_error(error, "could not open file for writing");
    
    fwrite(project->name, 8, 1, file);
    fwrite(&project->version, 1, 1, file);
    fwrite(project->compressed_data.data, project->compressed_data.size, 1, file);
    
    fclose(file);
}

void lsdj_read_lsdsng(const char* path, lsdj_project_t* project, lsdj_error_t** error)
{
	FILE* file = fopen(path, "r");
    if (file == NULL)
        return lsdj_create_error(error, "could not open file for reading");

    memset(project->name, 0, sizeof(project->name));
    if (project->compressed_data.size > 0)
    {
    	free(project->compressed_data.data);
    	project->compressed_data.size = 0;
    }

    fread(project->name, 8, 1, file);
    fread(&project->version, 1, 1, file);

    fseek(file, 0, SEEK_END);
    project->compressed_data.size = (size_t)(ftell(file) - 9);
    project->compressed_data.data = malloc(project->compressed_data.size);
    fseek(file, 0, SEEK_SET);

    fread(project->compressed_data.data, project->compressed_data.size, 1, file);
}
