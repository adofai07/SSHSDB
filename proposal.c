#include "proposal.h"
#include "typing.h"

int proposal_count = 0;
struct proposal proposals[MAX_PROPOSALS];

/// @brief Load proposals from proposal_list.txt
void load_proposals_from_file()
{
    FILE *file = fopen("proposal_list.txt", "r");
    if (file == NULL)
    {
        return; // If the file doesn't exist, we assume there are no saved proposals
    }

    proposal_count = 0;
    while (fscanf(file, "%d %d %[^\n]", &proposals[proposal_count].index, &proposals[proposal_count].sno, proposals[proposal_count].title) == 3)
    {
        proposal_count++;
    }

    fclose(file);
}

/// @brief Save a proposal to proposal_list.txt in append mode
/// @param new_proposal Pointer to the new proposal to be saved
void save_proposals_to_file(const struct proposal *new_proposal)
{
    FILE *file = fopen("proposal_list.txt", "a");
    if (file == NULL)
    {
        printf("Error opening proposal_list.txt file.\n");
        return;
    }

    fprintf(file, "%d %d %s\n", new_proposal->index, new_proposal->sno, new_proposal->title);

    fclose(file);
}

/// @brief Delete the oldest proposal from the proposals.txt file and update proposal_list.txt
void delete_oldest_proposal()
{
    load_proposals_from_file(); // Load proposals from file

    if (proposal_count == 0)
    {
        printf("No proposals to delete.\n");
        return;
    }

    // Update proposals.txt
    FILE *file = fopen("proposals.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    FILE *temp_file = fopen("temp_proposals.txt", "w");
    if (temp_file == NULL)
    {
        printf("Error opening temporary file.\n");
        fclose(file);
        return;
    }

    char buffer[MAX_TEXT_LENGTH];
    int skip_lines = 2; // Skip the first two lines (oldest proposal's title and text)

    // Copy all lines except the first two to a temporary file
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (skip_lines > 0)
        {
            skip_lines--;
        }
        else
        {
            fprintf(temp_file, "%s", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove("proposals.txt");
    rename("temp_proposals.txt", "proposals.txt");

    // Remove the oldest proposal from the proposals array and update indices
    for (int i = 1; i < proposal_count; i++)
    {
        proposals[i - 1] = proposals[i];
        proposals[i - 1].index = i - 1; // Update index
    }

    proposal_count--;

    // Update proposal_list.txt
    file = fopen("proposal_list.txt", "r");
    if (file == NULL)
    {
        printf("Error opening proposal_list.txt file.\n");
        return;
    }

    temp_file = fopen("temp_proposal_list.txt", "w");
    if (temp_file == NULL)
    {
        printf("Error opening temporary file.\n");
        fclose(file);
        return;
    }

    int skip_first_line = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (skip_first_line)
        {
            skip_first_line = 0;
        }
        else
        {
            // Parse the line to update the index
            int index, sno;
            char title[MAX_TITLE_LENGTH];
            sscanf(buffer, "%d %d %[^\n]", &index, &sno, title);
            index--; // Update index
            fprintf(temp_file, "%d %d %s\n", index, sno, title);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove("proposal_list.txt");
    rename("temp_proposal_list.txt", "proposal_list.txt");
}

/// @brief Scan a student's proposal and save it in proposals[] and "proposals.txt"
/// @param acc Pointer to account structure (student)
void scan_proposal(account_t *acc)
{
    load_proposals_from_file(); // Load proposals from file

    if (proposal_count >= MAX_PROPOSALS)
    {
        printf("Maximum number of proposals reached. Deleting the oldest proposal.\n");
        delete_oldest_proposal();
    }

    struct proposal new_proposal;
    new_proposal.index = proposal_count;
    new_proposal.sno = acc->student_no;

    // Scan the proposal title
    printf("Enter proposal title (max %d characters): ", MAX_TITLE_LENGTH);
    fgets(new_proposal.title, MAX_TITLE_LENGTH, stdin);
    new_proposal.title[strcspn(new_proposal.title, "\n")] = 0; // Remove newline

    // Scan the proposal text
    printf("Enter proposal text (max %d characters): ", MAX_TEXT_LENGTH);
    char *text = (char *)malloc(MAX_TEXT_LENGTH * sizeof(char));
    if (text == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    fgets(text, MAX_TEXT_LENGTH, stdin);
    text[strcspn(text, "\n")] = 0; // Remove newline

    proposals[proposal_count++] = new_proposal;

    // Append the title and text to the file
    FILE *file = fopen("proposals.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        free(text);
        return;
    }
    fprintf(file, "%s\n%s\n", new_proposal.title, text);
    fclose(file);

    free(text);

    // Save the new proposal to proposal_list.txt
    save_proposals_to_file(&new_proposal);
}

/// @brief Print 10 proposals' titles and indexes
/// @param page The page number to display
void print_proposal_list(int page)
{
    load_proposals_from_file(); // Load proposals from file

    int start = (page - 1) * 10;
    int end = start + 10;
    if (start >= proposal_count)
    {
        printf("No proposals to display.\n");
        return;
    }

    for (int i = start; i < end && i < proposal_count; i++)
    {
        printf("%d: %s\n", proposals[i].index, proposals[i].title);
    }
}

/// @brief Print a proposal's title, text, and student number (who proposed)
/// @param index The index of the proposal
void print_proposal(int index)
{
    load_proposals_from_file(); // Load proposals from file

    if (index < 0 || index >= proposal_count)
    {
        printf("Invalid proposal index.\n");
        return;
    }

    struct proposal *p = &proposals[index];

    FILE *file = fopen("proposals.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char buffer[MAX_TITLE_LENGTH + MAX_TEXT_LENGTH + 10];
    int found = 0;

    // Search for the proposal by title in the file
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char file_title[MAX_TITLE_LENGTH];
        char *file_text = (char *)malloc(MAX_TEXT_LENGTH * sizeof(char));
        if (file_text == NULL)
        {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }

        strcpy(file_title, buffer);
        file_title[strcspn(file_title, "\n")] = 0; // Remove newline

        // If the title matches, read and print the text
        if (strcmp(file_title, p->title) == 0)
        {
            found = 1;
            if (fgets(file_text, MAX_TEXT_LENGTH, file) != NULL)
            {
                file_text[strcspn(file_text, "\n")] = 0; // Remove newline
                printf("Proposal Index: %d\n", p->index);
                printf("Student No: %d\n", p->sno);
                printf("Title: %s\n", p->title);
                printf("Text: %s\n", file_text);
            }
            free(file_text);
            break;
        }
        free(file_text);
    }

    if (!found)
    {
        printf("Proposal not found in the file.\n");
    }

    fclose(file);
}
