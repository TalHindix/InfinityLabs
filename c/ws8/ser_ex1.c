#include <stdio.h>
#include <string.h>

typedef struct 
{
    float sociology; 
    float psychology;  
} HumanitiesGrades_t;

typedef struct 
{
    float math;
    float physics;
    float chemistry;
} ScienceGrades_t;


typedef struct 
{
    HumanitiesGrades_t humanities; 
    ScienceGrades_t science;      
    float sports;                 
} GradeRecord_t;


typedef struct 
{
    char first_name[50];  
    char last_name[50]; 
    GradeRecord_t record; 
} Student_t;


int Save_Student_Binary(const char *filepath, const Student_t *student)
{
    FILE *out = fopen(filepath, "wb");
    
    if (out == NULL) 
    {
        perror("Error opening file for write");
        return 0;
    }

    if (fwrite(student, sizeof(Student_t), 1, out) != 1) 
    {
        perror("Error writing student record");
        fclose(out);
        return 0;
    }

    fclose(out);
    return 1;
}


int Load_Student_Binary (const char *filepath, Student_t *out_student)
{
    FILE *in = fopen(filepath, "rb");
    
    if (in == NULL) 
    {
        perror("Error opening file for read");
        return 0;
    }

    if (fread(out_student, sizeof(Student_t), 1, in) != 1) 
    {
        perror("Error reading student record");
        fclose(in);
        return 0;
    }

    fclose(in);
    return 1;
}

int main()
{
	Student_t tal = {0};
    Student_t loaded_tal = {0};

    strcpy(tal.first_name, "Tal");
    strcpy(tal.last_name, "Hindi");

    tal.record.science.math = 99.5;
    tal.record.science.physics = 99.0;
    tal.record.science.chemistry = 97.3;

    tal.record.humanities.sociology = 88.0;
    tal.record.humanities.psychology = 90.0;

    tal.record.sports = 91.0;

	if (Save_Student_Binary("tal.bin", &tal))
	{
        printf("Successfully saved student data.\n");
    }
    
    if (Load_Student_Binary("tal.bin", &loaded_tal))
    {
        printf("Loaded Student: %s %s\n",loaded_tal.first_name, loaded_tal.last_name);
        printf("Math: %f, Sports: %f\n",loaded_tal.record.science.math,loaded_tal.record.sports);
    }
    
    if (memcmp(&tal, &loaded_tal, sizeof(Student_t)) == 0)
	{
		printf("PASS: everything equal");
	}
	else
	{
		printf("FAIL: structs not equal");
	}
    
    

	return 0;
}
