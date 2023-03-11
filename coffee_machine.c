/*******************************************************************************
 COMPILE: gcc -o coffee_machine coffee_machine.c -ldialog
 RUN: ./dialog_menu_example
 DOCUMENTATION: https://invisible-island.net/dialog/manpage/dialog_lib.html
 AUTHOR: Vitaly Castaño Solana
 LICENCE: MIT
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dialog.h>
#include <string.h>
#include <math.h>
//#include <stdint.h> // To ensure your code is portable, we can use types defined in the standard headers such as <stdint.h>

/************************* GET TOTAL ARRAY LENGTH ******************************
 sizeof(arr) = gets total size of an array in bytes
 sizeof(arr)[0] = gets total size in bytes of just one element in an array

 EXAMPLE:( (unsigned int)(sizeof(coffee_options) / sizeof(coffee_options)[0])) );
		 ( (unsigned int)(128 bytes / 8 bytes) )

 EXAMPLE: // get the size of array
    int size = sizeof(arr) / sizeof(arr[0]);
*******************************************************************************/
#define GET_ARRAY_LENGTH(arr) ((int) (sizeof(arr) / sizeof(arr)[0]))

// comfortably modify coffees prices here only (cents only)
//#define DRIP_COFFEE_PRICE	"1.78" // coffee_price = roundf(atof(DRIP_COFFEE_PRICE) * 100.0) / 100.0;
#define DRIP_COFFEE_PRICE	178
#define ESPRESSO_PRICE		144
#define MACCHIATO_PRICE		124
#define AMERICAN_PRICE		115
#define LATTE_PRICE			119
#define CAPUCCINO_PRICE		187
#define MOCHA_PRICE			151
#define COIN				"$"
#define CENT				"¢"

// Same for indgredients
#define SUGAR_PRICE		20
#define MILK_PRICE		30
#define SWEETENER_PRICE	14
#define ICE_PRICE		5

/************************* MACRO TO STRING COVERTER ****************************
 - STR_AUX(x) #x
	aux macro that gets a value of another macro as argument, and then converts
	it into a string using '#' operator. This stringing operator that converts
	an expression into a string.
 - EXAMPLE: #define STR_AUX(x) #x converts macro COIN into "COIN"

 - STR(x) calls the aux macro STR_AUX(x) with 'x' value as argument.
 - EXAMPLE: #define STR(COIN) STR_AUX(COIN) converts the macro COIN into "COIN"

 - EXAMPLE: STR(DRIP_COFFEE_PRICE) COIN
*******************************************************************************/
#define STR_AUX(x) #x
#define STR(x) STR_AUX(x)

#define CLEAR_TERMINAL system("clear") // clear terminal screen when program finalizes

// GLOBAL VARS
// We can’t use any modifiers in float data type. If programmer tries to use it,
// the compiler automatically gives compile time error.
//int coffee_price; // standalone coffee price
//int ingredients_price = 0; // sum ofingredients price
//float dollars;
//char* selected_coffee_type = NULL;

/*static void ClearScreen(void) // clear dialog's screen
{
	dlg_clear();
	dlg_put_backtitle();
}*/

/*void ResultInputClear(void)
{
    if (dialog_vars.input_length)
    {
        dialog_vars.input_length = 0;
    }
    if (dialog_vars.input_result)
    {
        free(dialog_vars.input_result);
    }
    dialog_vars.input_result = 0;
}*/

char* PrintDollars(int cents)
{
    float dollars = cents / 100.0;
    //int cents = cents % 100;
    char* str_dollars = malloc(sizeof(char) * 14); // adjust for your own needs
    sprintf(str_dollars, "%.2f%s", dollars, COIN);
    return str_dollars;
}

void ByeMessage() // just a bye message on program exit/finish
{
    dialog_vars.no_lines = 1; // remove decorated border lines
    init_dialog(stdin, stdout); // input + output dialog init
    dialog_msgbox( // show messagebox
        "Bye", // title (NULL is not title)
        "See you soon", // message
        0, // size (0 is auto)
        0, // size (0 is auto)
        0); // 1 if you want show Accept button for continue
    usleep(1500000); // pause for few seconds, to see the message, like infobox
    end_dialog(); // stop dialog
    //clear_screen();
    CLEAR_TERMINAL;
    exit(0); // finalize the program, quit
}
/*
unsigned short AcceptCancel(unsigned short retval) // control return value for Accept/Cancel buttons
{
    // what to do if Accept or Cancel is pressed
    switch(retval)
    {
    case 0: // Accept button is pressed
        printf("Accept is pressed\n");
        break;
    case 1: // Cancel button is pressed
        printf("Cancel is pressed\n");
        break;
    case 2: // Cancel button is pressed
        printf("Exit is pressed\n");
        ByeMessage();
        break;
    default: // something gone wrong
        printf("Invalid add option\n");
        break;
    }

    return retval;
}*/

void InsertMoney(int temp_money) // function for showing the total price amount
{
    //dialog_vars.no_lines = 1;
    char temp_char[100];
    //temp_char[] = PrintDollars(total_cents_price);
    sprintf(temp_char, "Please, insert %d.%02d%s", temp_money / 100, temp_money % 100, COIN);
    //sprintf(temp_char, "Please, insert %d%s", temp_money, COIN);
    init_dialog(stdin, stdout); // input + output

    dialog_msgbox(
        "Insert money",
        temp_char,
        0,
        0,
        1); // 1 if you want show Accept button for continue
    //usleep(900000); // pause for few seconds, to see the welcome message, like infobox
    end_dialog();
}

void WellcomeMessage() // just a wellcome message
{
    init_dialog(stdin, stdout); // input + output
    dialog_vars.no_lines = 1;
    dialog_vars.backtitle= "Main menu";
    dialog_msgbox(
        NULL,
        "Happy drinking",
        0,
        0,
        0); // 1 if you want show Accept button for continue
    usleep(900000); // pause for few seconds, to see the welcome message, like infobox
    end_dialog();
}

void Menu() // main menu
{
    //unsigned short coffee_accepted = 1; // Cancel
    //unsigned short ingredients_accepted = 1;
    //int coffee_price; // standalone coffee price
    int total_cents_price; // sum of coffee price + ingredients
    //char *selected_coffee_type = NULL;

    //dialog_vars.cancel_label = "Exit"; // set Cancel button to Exit, in main menu
    //char *selected_ingredients; // variable for storing the selected ingredients, out of do/while loop for reusing
    //int checklist_result; // result (0 or 1), out of do/while loop for reusing

    short get_back = 0;

    char *coffee_options[] = // Coffees types
    {
        // identifier option char, description,
        // must have TAG, ITEM, HELP(optional)
        "1 " "Drip coffee",	PrintDollars(DRIP_COFFEE_PRICE),
        "2 " "Espresso",	PrintDollars(ESPRESSO_PRICE),
        "3 " "Macchiato",	PrintDollars(MACCHIATO_PRICE),// STR(MACCHIATO_PRICE) COIN,
        "4 " "American",	PrintDollars(AMERICAN_PRICE),// PrintDollars(AMERICAN_PRICE)
        "5 " "Latte",		PrintDollars(LATTE_PRICE),
        "6 " "Capuccino",	PrintDollars(CAPUCCINO_PRICE),
        "7 " "Mocha",		PrintDollars(MOCHA_PRICE)
    };

    char *ingredients[] = // ingredients addons
    {
        //{tag,  item,  status}
        "1", " Sugar     " 	STR(SUGAR_PRICE) CENT,		"off", // "on" if you want this selected by default on start
        "2", " Milk      " 	STR(MILK_PRICE) CENT,		"off",
        "3", " Sweetener "	STR(SWEETENER_PRICE) CENT,	"off",
        "4", " Ice       " 	STR(ICE_PRICE) CENT,		"off"
    };

    do
    {
        get_back = 0;
        dialog_vars.cancel_label = "Exit";
        // initialize dialog lib
        //init_dialog(stdin, stdout);

        //dialog_state.no_mouse = 1; // disable mouse input
        //dialog_vars.item_help = 0; // for checklist, radiolist and menu boxes adding a column whose text is displayed in the bottom line of the screen
        //dialog_vars.default_button = 0;  // Focus on "OK"
        //dialog_vars.extra_button = 1; // enable an extra button
        //dialog_vars.extra_label = "Help"; // extra button's label
        //dialog_vars.no_items = 0; // This tells dialog to read shorter rows from data, omitting the list
        dialog_vars.no_lines = 0; // removes box line drawing
        dialog_vars.backtitle = "Main menu"; // backtitle string to be displayed on the backdrop, at the top of the screen
        //dialog_vars.default_item=strdup("1 Drip coffee");
        //dialog_vars.beep_after_signal = 1;
        //dialog_vars.exit_label = "Exit";
        //dialog_vars.extra_button = 1;
        //dialog_vars.extra_label = "Exit";
        //dialog_vars.cancel_label = "Exit";
        //dialog_vars.input_result = "";

        // menu options pointer
        /*char *coffee_options[] =
        { // identifier option char, description,
        	"1", "Drip coffee",
        	"2", "Espresso",
        	"3", "Macchiato",
        	"4", "American",
        	"5", "Latte",
        	"6", "Capuccino",
        	"7", "Mocha"
        };*/

        //float drip_coffee_f = printf("%.2f", PrintDollars(DRIP_COFFEE_PRICE));

        //printf("%s ($%.2f)\n", coffee_options[0], AMERICANO_PRICE);
        //printf("%s ($%.2f)\n", coffee_options[1], LATTE_PRICE);
        //printf("%s ($%.2f)\n", coffee_options[2], MOCHA_PRICE);

        // initialize dialog lib
        init_dialog(stdin, stdout);
        // show the main menu and get the int number as result
        if (dialog_menu(
                    "Coffee machine", // title
                    "Select your coffee type", // text prompt
                    0, // height
                    0, // width
                    0, // menu height
                    GET_ARRAY_LENGTH(coffee_options) / 2, // number of options, 2 divider because we need to get real available options, we have 2 strings for every coffee option, identifier and name
                    coffee_options) // the list of options
                == DLG_EXIT_CANCEL)
        {
            printf("DLG_EXIT_CANCEL = %d", DLG_EXIT_CANCEL); // on Exit button press (1)
            ByeMessage();
        }
        else // on Exit button press (0)
        {
            printf("else DLG_EXIT_CANCEL = %d\n", DLG_EXIT_CANCEL);
        }
        end_dialog();
        // because dialog_menu function returns 0 or 1, we need to know what option
        // user selected, and then store the char identifier into *result pointer
        // dialog_vars.input_result == "1 Drip coffee"
        char *selected_coffee_type = dialog_vars.input_result; // this stores a char ('1', '2', '3', ... )
        //char selected_coffee_name[50] = {dialog_vars.input_result}; // this stores a char ('1', '2', '3', ... )

        /*if (selected_coffee_type == NULL)
        {
        	printf("Por favor, seleccione un tipo de café.\n");
        	continue;
        }*/

        printf("*selected_coffee_type is = %c\n", *selected_coffee_type); // = "1"
        printf("selected_coffee_type is = %s\n", selected_coffee_type); // = "1 Drip coffee"
        /*switch (selected_option)
        {
        	int i_temp;
        	char c;
        	char c_temp[30];
        	// Generate a case for all options inside array coffee_options[]
        	for (int i = 1; i < GET_ARRAY_LENGTH(coffee_options) / 2; i++)
        	{
        		i_temp = i;
        		c = i + '0';
        		//c = printf("%s", c);
        		c = printf("%s\n", (char[]){'\'', c, '\'', ':', '\0'});
        		case c:
        			printf("User selected option %s: %s\n", coffee_options[i * 2], coffee_options[i * 2 + 1]);
        			break;
        	}
        	// Control any other not valid case
        	default:
        		printf("Invalid option\n");
        		break;
        }*/

        /*for (int i = 0; i < GET_ARRAY_LENGTH(coffee_options) / 2; i++)
        {
        	if (!strcmp(items[i], dialog_vars.input_result))
        	{
        		procedure = procedures[i];
        		break;
        	}
        }*/


        switch(*selected_coffee_type)
        {
        case '1': // char '1'
            printf("User selected Drip Coffee\n");
            total_cents_price = DRIP_COFFEE_PRICE;
            break;
        case '2': // char '2'
            printf("User selected Espresso\n");
            total_cents_price = ESPRESSO_PRICE;
            break;
        case '3': // char '3'
            printf("User selected Macchiato\n");
            total_cents_price = MACCHIATO_PRICE;
            break;
        case '4': // char '4'
            printf("User selected American\n");
            total_cents_price = AMERICAN_PRICE;
            break;
        case '5': // char '5'
            printf("User selected Latte\n");
            total_cents_price = LATTE_PRICE;
            break;
        case '6': // char '6'
            printf("User selected Capuccino\n");
            total_cents_price = CAPUCCINO_PRICE;
            break;
        case '7': // char '7'
            printf("User selected Mocha\n");
            total_cents_price = MOCHA_PRICE;
            break;
        default: // something gone wrong
            printf("Invalid coffee option\n");
            //coffee_accepted = 1;
            break;
        }

        //accept_cancel_coffee = AcceptCancel(accept_cancel_coffee);
        /*if (accept_cancel_coffee == 1)
        {
        	ByeMessage();
        }*/

        // what to do if Accept or Cancel is pressed
        /*switch(accept_cancel_coffee)
        {
        	case 0: // Accept button is pressed
        		printf("Accept is pressed\n");
        		break;
        	case 1: // Cancel button is pressed
        		printf("Cancel is pressed\n");
        		ByeMessage();
        		break;
        	default: // something gone wrong
        		printf("Invalid add option\n");
        		break;
        }*/
        //ResultInputClear();
        //free(dialog_vars.input_result);

        //dialog_vars.input_length = 0;
        dialog_vars.input_result = NULL; // clear the previous dialog widget selectiong output
        //free(dialog_vars.input_result);

        //dlg_clr_result();

        init_dialog(stdin, stdout); // input + output dialog init
        dialog_vars.cancel_label = "Back";
        if (dialog_checklist( // create a checklist with the defined options
                    "Do you want some ingredient?", // title
                    "Select ingredients you want:", // message
                    0, // height
                    0, // width
                    0, // menu height
                    GET_ARRAY_LENGTH(ingredients) / 3, // array length
                    ingredients, // array with options
                    FLAG_CHECK)
                == DLG_EXIT_CANCEL) // here we want a checklist instead of radiobutton, because with radiobuttons only one option is possible
        {
            printf("DLG_EXIT_CANCEL = %d", DLG_EXIT_CANCEL); // on Back button press (1)
            get_back = 1;
            //end_dialog(); // stop dialog
            //continue;
        }
        else if (dialog_vars.input_result != NULL)
        {
            char *selected_ingredients = dialog_vars.input_result; // variable for storing the selected options, out of do/while loop for reusing
            printf("*selected_ingredients is = %s\n", selected_ingredients);
            //char *selected_options = dialog_vars.input_result;
            //printf("Opciones seleccionadas: %s\n", selected_options);

            // here we get the first token/string/char from "selected_options" variable,
            // we use space as delimiter (if more than one option stored)
            char *token = strtok(selected_ingredients, " ");
            //printf("*token = %s\n", token);
            int selected_indexes[GET_ARRAY_LENGTH(ingredients) / 3]; // an temp array
            int i = 0;
            while (token != NULL)
            {
                // -1 because array starts at 0
                int option_index = atoi(token) - 1; // char to int (option 1 stores as 0 because of -1)
                //printf("111 option_index = %d\n", option_index);
                // this will print names of the selected options
                //printf("%s\n", options[option_index * 3 + 1]); // print option name
                selected_indexes[i] = option_index;
                //printf("selected_indexes[%d] = %d\n", i, selected_indexes[i]);
                i = i + 1;
                // para dividir la cadena dialog_vars.input_result en subcadenas separadas por comas
                token = strtok(NULL, " ");
            }

            // run the code for every selected option by user
            for (int y = 0; y < i; y++)
            {
                //int option_index = selected_options_indices[i];
                switch (selected_indexes[y])
                {
                case 0:
                    // run option 1
                    printf("Code to run for option 0\n");
                    total_cents_price = total_cents_price + SUGAR_PRICE;
                    break;
                case 1:
                    // run option 2
                    printf("Code to run for option 1\n");
                    total_cents_price = total_cents_price + MILK_PRICE;
                    break;
                case 2:
                    // run option 3
                    printf("Code to run for option 2\n");
                    total_cents_price = total_cents_price + SWEETENER_PRICE;
                    break;
                case 3:
                    // run option 4
                    printf("Code to run for option 3\n");
                    total_cents_price = total_cents_price + ICE_PRICE;
                    break;
                default:
                    // invalid option
                    printf("Invalid option\n");
                    break;
                }
            }
        }

        printf("Total price: %d", total_cents_price);

        /*else //if (DLG_EXIT_CANCEL == 0)
        {
        	printf("else if DLG_EXIT_CANCEL = %d", DLG_EXIT_CANCEL); // on Accept button press

        }*/
        end_dialog(); // stop dialog
        //printf("DLG_EXIT_CANCEL = %d", DLG_EXIT_CANCEL);
        // Finalize dialog
        //end_dialog();
        //printf("accept_cancel_coffee = %d\n", accept_cancel_coffee);

        //init_dialog(stdin, stdout);
        //clear_screen();
    }
    while(get_back == 1);  // repeat function when user press Cancel button
    /*
        do
        {
            init_dialog(stdin, stdout); // input + output dialog init
            dialog_vars.cancel_label = "Back";
            dialog_vars.input_result = NULL;
            checklist_result = dialog_checklist( // create a checklist with the defined options
                                   "Choose your colors", // title
                                   "Select at least one color:", // message
                                   0, // height
                                   0, // width
                                   0, // menu height
                                   GET_ARRAY_LENGTH(ingredients) / 3, // array length
                                   ingredients, // array with options
                                   FLAG_CHECK); // here we want a checklist instead of radiobutton, because with radiobuttons only one option is possible

            end_dialog(); // stop dialog
        }while(checklist_result != 0);*/

    /*if (checklist_result == -1)
    {
        continue;
    }*/

    //bool selected_ingredients = dialog_vars.separate_output;
    //ingredients_accepted = AcceptCancel(ingredients_accepted);

    //InsertMoney(total_cents_price);

    InsertMoney(total_cents_price);
    //init_dialog(stdin, stdout);
    //dialog_menu("my title", strcmp(selected_coffee_type, "1") ? "Two" : "One", 0, 0, 0, 3, coffee_options);
    //end_dialog();

    //printf("Array size coffee_options[] = %d\n", GET_ARRAY_LENGTH(coffee_options));
    //printf("sizeof(coffee_options) = %d bytes\n", sizeof(coffee_options));
    //printf("sizeof(coffee_options)[0] = %d bytes\n", sizeof(coffee_options)[0]);
    //printf("We have %d types of coffees available\n", ((unsigned int)(sizeof(coffee_options) / sizeof(coffee_options)[0]) / 2) );
}

int main()
{
    //int ingredients_price = 0; // sum ofingredients price
    //float dollars;

    WellcomeMessage();
    Menu();
    ByeMessage();
    //clear_screen();
    end_dialog();
    CLEAR_TERMINAL;

    return 0;
}
