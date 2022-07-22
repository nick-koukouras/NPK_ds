# NPK Data Structures
This Single Header File can be used very easily by just including it in any of your c files. Like all Single Header Files there is no need to configure your environment, it just works! To top it off, it was designed from scratch to be simple and intuitive to use with a permissive license.

Here are some examples that showcase the three data structures that this library contains (generic dynamic arrays, strings and generic maps). Have fun!

1. Dynamic array example with the type chosen to be an int (try it with other types!):
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

int main(int argc, char **argv) {
    // Create an array of integers
    NPK_array ints = NPK_array_new(int);

    // Loop with variable i -> 0, 1, 2, 3, 4
    NPK_count_up (i, 0, 5) { // NPK_count_down (i, 5, 0) would loop with i -> 4, 3, 2, 1, 0
        NPK_array_add(int, &ints, i * 5); // Add to the array the values of i*5
    }

    // Print the length of the array
    printf("%d\n", NPK_array_length(&ints)); // Output: 5

    // A for_each loop
    NPK_for_each (int, elem, &ints) {
        printf("%d ", *elem); // Output: 0, 5, 10, 15, 20
    }

    // Replace ALL values equal to 10 with value 50 in the array
    // NPK_int_equals is the way it's gonna check for equality between integers
    NPK_array_replace(int, &ints, 10, 50, NPK_int_equals);

    // Print the value of the array at index 2
    printf("%d\n", NPK_array_data(int, &ints)[2]); // Output: 50

    // Sort the array in ascenting order
    // For descenting order, we would replace NPK_int_asc with NPK_int_desc
    NPK_array_sort(int, &ints, NPK_int_asc); // The array now contains [0, 5, 15, 20, 50]

    // Reverse the array
    NPK_array_reverse(int, &ints); // The array now contains [50, 20, 15, 5, 0]

    // Remove the element of the array at index 1
    NPK_array_remove_at(int, &ints, 1); // The array now contains [50, 15, 5, 0]

    // Remove ALL values equal to 50 in the array
    NPK_array_remove(int, &ints, 50, NPK_int_equals); // The array now contains [15, 5, 0]

    // Check if the array contains the value 10
    if (NPK_array_contains(int, &ints, 10, NPK_int_equals)) {
        // Since value 10 currently is not contained in the array,
        // this block will NOT be executed
    }

    // Check if the array is empty
    if (NPK_array_is_empty(&ints)) {
        // Since the array has 3 elements, this block will NOT be executed
    }

    // Create a copy of the array
    // The array will contain the same elements [15, 5, 0]
    NPK_array ints_copy = NPK_array_copy_new(int, &ints);

    // Check if two arrays are equal
    if (NPK_array_equals(int, &ints, &ints_copy, NPK_int_equals)) {
        // Well, this block will be executed... Obviously :D
    }

    // Replace ONE element of the array at a certain index
    // For example at index 0
    NPK_array_data(int, &ints)[0] = 20; // The array now contains [20, 5, 0]

    // Delete our arrays
    NPK_array_del(&ints_copy);
    NPK_array_del(&ints);
}
```
2. A similar example for string, if everything above was clear, this should be a piece of cake:
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

int main(int argc, char **argv) {
    NPK_cast(void, argc);
    NPK_cast(void, argv);

    // Create a string
    // Note: NPK_string is an NPK_array of type char that always ends in '\0'
    // A simple NPK_array of type char doesn't contain '\0' and that is their difference
    NPK_string str = NPK_string_new("abcde"); // The string is now "abcde"

    // Add a character
    NPK_string_add(&str, 'f'); // The string is now "abcdef"

    // Loop through each character
    // The for_each loop doesn't repeat for the '\0' character
    NPK_for_each (char, c, &str) {
        printf("%c", *c); // Output: abcdef
    }

    // Print the length of the string (doesn't count the '\0' character)
    // To count the '\0' character, treat the string as an array
    // Example: NPK_array_length(&str)
    printf("%d\n", NPK_string_length(&str)); // Output: 6

    // Replace ALL values equal to "abc" with value "ABC" in the string
    NPK_string_replace(&str, "abc", "ABC"); // The string now is "ABCdef"

    // Print the character of the string at index 2
    printf("%c\n", NPK_string_data(&str)[2]); // Output: C

    // Print the whole string (intuitively, just omit the index)
    printf("%s\n", NPK_string_data(&str)); // Output: ABCdef

    // Sort the string in descenting order
    NPK_string_sort(&str, NPK_char_desc); // The string now is "fedCBA"

    // Reverse the string
    NPK_string_reverse(&str); // The string now is "ABCdef"

    // Remove the element of the array at index 1
    NPK_string_remove_at(&str, 1); // The string now is "ACdef"

    // Remove ALL values equal to "AC" in the string
    NPK_string_remove(&str, "AC"); // The string now is "def"

    // Append another string
    NPK_string_append(&str, "ghi"); // The string now is "defghi"

    // Check if the string contains the value "fg"
    if (NPK_string_contains(&str, "fg")) {
        // Since value "fg" is currently contained in the string,
        // this block will be executed
    }

    // Check if the string is NOT empty
    // A string is empty when it only contains '\0'
    if (NPK_string_is_empty(&str) == false) {
        // Since the string has 6 elements (without counting '\0'),
        // this block will be executed
    }

    // Check if string starts with "def"
    // There also exists the equivalent NPK_string_ends()
    if (NPK_string_starts(&str, "def")) {
        // This block will be executed
    }

    // Create a copy of the string
    NPK_string str_copy = NPK_string_copy_new(&str); // The string contains the same value "defghi"

    // Replace ONE character of the string at a certain index
    // For example at index 0
    NPK_string_data(&str)[0] = 'D'; // The string now contains the value "Defghi"

    // Make the string uppercase, for lowercase use NPK_string_to_lower(&str)
    NPK_string_to_upper(&str); // The string now contains "DEFGHI"

    // Check if two strings are equal
    // Notice how NPK_string_data allows us to use a second variable,
    // instead of a hardcoded value in quotes like all other cases above
    if (NPK_string_equals(&str, NPK_string_data(&str_copy))) {
        // This block will NOT be executed...
        // "DEFGHI" and "defghi" are not considered the same
    }

    // Delete our strings
    NPK_string_del(&str_copy);
    NPK_string_del(&str);
}
```
3. A simple example with a map that has keys of type int and values of type double:
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

int main(int argc, char **argv) {
    NPK_cast(void, argc);
    NPK_cast(void, argv);

    // Create a map that accepts values of type double
    NPK_map m = NPK_map_new(double);

    // Create 3 keys, they can be of any type
    // The reason is that the map actually works with their address
    // Hence, only a void pointer that points at each key is stored
    int k[3];

    // Add 3 values to the map
    // Notice that the keys can be used uninitialized
    // The keys could have values, it doesn't matter
    NPK_map_add(double, &m, &k[0], 1.1);
    NPK_map_add(double, &m, &k[1], 2.2);
    NPK_map_add(double, &m, &k[2], 3.3);

    // Pretending we don't already have the keys available
    // Let's obtain them, this also puts the in an NPK_array
    NPK_array keys = NPK_map_keys_new(&m);

    // Since we have the keys in an NPK_array, we can use a for_each loop :)
    NPK_for_each (void *, key, &keys) {
        // Let's get the value of each key, wrong keys return null
        // Notice that we are dereferencing the pointer to the value returned
        // If unsure about the key, check that the pointer isn't null first
        double val = *NPK_map_get(double, &m, *key);

        printf("%lf ", val); // Output: 1.1 2.2 3.3 in any order
    }

    // We could also retrieve all the values of the map
    NPK_array vals = NPK_map_values_new(double, &m); // Contains 1.1 2.2 3.3 in any order

    // Print the length of the map
    printf("%d\n", NPK_map_length(&m)); // Output: 3

    // Remove a key-value pair using a key
    NPK_map_remove(&m, &k[1]); // Will remove k[1] along with value 2.2

    // Check if the map contains the value 2.2
    if (NPK_map_contains_value(double, &m, 2.2, NPK_dbl_equals)) {
        // Since value 2.2 is NOT currently contained in the map,
        // this block will NOT be executed
    }

    // Check if the map contains key k[1]
    if (NPK_map_contains_key(&m, &k[1])) {
        // Since we removed k[1], this block will NOT be executed
    }

    // Check if the map is NOT empty
    if (NPK_map_is_empty(&m) == false) {
        // Since the map has 2 elements, this block will be executed
    }

    // Create a copy of the map
    NPK_map m_copy = NPK_map_copy_new(double, &m); // The map contains the same value key-values

    // Check if two maps are equal
    if (NPK_map_equals(double, &m, &m_copy, NPK_dbl_equals)) {
        // This block will be executed :)
    }

    // Delete our maps
    NPK_map_del(&m_copy);
    NPK_map_del(&m);
}
```
When working with multidimensional arrays, the number of dimensions might be required as a parameter. Always check if an equivalent function or macro named NPK\_array\_dim\_\* exists. Using a multidimensional array on NPK\_array\_del instead of NPK\_array\_dim\_del is wrong. But using a multidimensional array on NPK\_array\_add is not, since NPK\_array\_dim\_add doesn't exist. The reason is that NPK\_array\_add doesn't need additional information to complete its operation.

When working with multidimensional arrays, you need to understand what a short copy and a deep copy is. A shallow copy of pointer A is another pointer B, pointing to wherever A points at. A deep copy of a pointer A, is another pointer B, pointing at a copy of data that A points at. This means that with a shallow copy we have ptr1 == ptr2, unlike with a deep copy. Likewise, this would be considered a check for shallow equality since it only checks if the pointers point at the same address. Performing obj1 = obj2 on objects that contain pointers, also creates a shallow copy of the pointers inside. And if you haven't guessed it already, checking obj1 == obj2 in such a scenario is equivalent to shallow equality. All of this is important because deallocating two pointers that were shallowly copied, will result in double free.

Each dimension of this library's array is allocated dynamically. Remember, strings are arrays themselves. Maps are made of two arrays, named keys and values. The keys array is always 1D and the values array is usage dependent. Using multidimensional arrays means that (through the library) you have to deallocate all dimensions properly. This means no memory leaks and no double frees :D.

4. With that out of the way let's see an example of multidimensional arrays:
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

int main(int argc, char **argv) {
    // Create a 2D array
    NPK_array strs = NPK_array_new(NPK_string);

    // Add strings to the array
    // Adding elements to the array is done through shallow copying
    // Deleting a multidimensional array deallocates its elements as well
    NPK_array_add(NPK_string, &strs, NPK_string_new("abc"));
    NPK_array_add(NPK_string, &strs, NPK_string_new("def"));
    NPK_array_add(NPK_string, &strs, NPK_string_new("ghi"));

    // Replace a string in a 2D array
    // ==========================================
    NPK_string from = NPK_string_new("abc");
    NPK_string to = NPK_string_new("ABC");

    // When deep equality between elements is found
    // 1. The element that is deeply equal to parameter from is deallocated
    // 2. In its place a deep copy of the parameter to, is made
    // The array now is: ["ABC", "def", "ghi"]
    NPK_array_dim_replace(char, &strs, &from, &to, NPK_char_equals, 2);

    // Both from and to, will be deallocated
    // Variable from wasn't copied at all and variable to was copied deeply
    NPK_array_del(&from);
    NPK_array_del(&to);
    // ==========================================

    // Sort the 2D array in descenting order
    // The array now is: ["CBA", "fed", "ihg"]
    NPK_array_dim_sort(char, &strs, NPK_char_desc, 2);

    // Remove "CBA" from the array and deallocate the element
    NPK_string str = NPK_string_new("CBA");
    NPK_array_dim_remove(char, &strs, &str, NPK_char_equals, 2);
    NPK_string_del(&str); // str must be deallocated

    // Check with deep equality if "fed" is contained in the array
    NPK_string str2 = NPK_string_new("fed");
    if (NPK_array_dim_contains(char, &strs, &str2, NPK_char_equals, 2)) {
        // This block will be executed
    }

    NPK_string_del(&str2); // str2 must be deallocated

    // Create a deep copy of a 2D array
    NPK_array strs_copy = NPK_array_dim_copy_new(char, &strs, 2);

    // Check for deep equality
    if (NPK_array_dim_equals(char, &strs, &strs_copy, NPK_char_equals, 2)) {
        // This block will be executed
    }

    // Delete the multidimensional arrays
    // This means deleting every shallow addition made
    // If you somehow made a deep addition, only the element added will be deleted
    NPK_array_dim_del(&strs_copy, 2);
    NPK_array_dim_del(&strs, 2);
}
```
5. Now an example about splitting an array. Splitting an array always results in a multidimensional array:
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

int main(int argc, char **argv) {
    // Create an array of int
    NPK_array arr = NPK_array_new(int);

    // Add some sample elements
    // ========================
    NPK_array_add(int, &arr, 1);
    NPK_array_add(int, &arr, 2);
    NPK_array_add(int, &arr, 3);

    NPK_array_add(int, &arr, 99);

    NPK_array_add(int, &arr, 4);
    NPK_array_add(int, &arr, 5);
    NPK_array_add(int, &arr, 6);

    NPK_array_add(int, &arr, 99);

    NPK_array_add(int, &arr, 7);
    NPK_array_add(int, &arr, 8);
    NPK_array_add(int, &arr, 9);
    // ========================

    // Split the array at every element with value 99
    // This means that we get separate parts delimited by 99
    // In our example that means: [ [1, 2, 3], [4, 5, 6], [7, 8, 9] ]
    // Notice that the array holding the parts (also arrays) is exactly one dimension higher
    NPK_array arr2d = NPK_array_split_new(int, &arr, 99, NPK_int_equals);

    // Traverse a 2D array
    NPK_for_each (NPK_array, larr, &arr2d) {
        // Output:
        // 1 2 3 (1st iteration)
        // 4 5 6 (2nd iteration)
        // 7 8 9 (3rd iteration)
        NPK_for_each (int, elem, larr) {
            printf("%d ", *elem);
        }

        printf("\n");
    }

    // Delete everything
    NPK_array_del(&arr);
    NPK_array_dim_del(&arr2d, 2);

    // Create a string
    NPK_string str = NPK_string_new("abcZZdefZZghi");

    // Split a string
    // Unlike spliting an array of chars, our delimiter doesn't need to be a single char
    // Result: ["abc", "def", "ghi"]. Again, as always, one dimension higher
    NPK_array strs = NPK_string_split_new(&str, "ZZ");

    // Traverse a 2D array
    NPK_for_each (NPK_string, lstr, &strs) {
        // Output:
        // abc
        // def
        // ghi
        printf("%s\n", NPK_string_data(lstr));
    }

    // Delete everything
    NPK_string_del(&str);
    NPK_array_dim_del(&strs, 2);
}
```
6. Finally an example of arrays holding objects of your own. I leave it as an exercise to the reader about how to deal with multidimensional arrays that hold objects:
```C
#define NPK_DS_IMPL // This line is required exactly once
#include "NPK_ds.h" // This line will still be required every other time you use the library

// Our struct for creating person objects
typedef struct {
    NPK_string name;
    NPK_string surname;
    int age;
} person;

// Since a person object is going to hold strings, we need to be able to deallocate them
void person_del(person *p) {
    NPK_string_del(&p->name);
    NPK_string_del(&p->surname);
}

// We also need a way to deallocate arrays that hold person objects
void array_person_del(NPK_array *arr) {
    NPK_for_each (person, p, arr) {
        person_del(p);
    }

    // Notice that we just extended the functionality of NPK_array_del,
    // we didn't replicate it, or replace it. There was no way for NPK_array_del,
    // to know all the future objects it would hold anyway.. Same thing about NPK_array_dim_del
    NPK_array_del(arr);
}

// Since a person object is going to hold strings, we need to be able to copy them
person person_copy_new(const person *p) {
    return (person) {NPK_string_copy_new(&p->name), NPK_string_copy_new(&p->surname), p->age};
}

// Now we also need a way to copy arrays holding person objects
NPK_array array_person_copy_new(const NPK_array *arr) {
    NPK_array arr_copy = NPK_array_new(person);

    NPK_for_each (person, p, arr) {
        NPK_array_add(person, &arr_copy, person_copy_new(p));
    }

    return arr_copy;
}


// Check for equality between person objects
bool person_equals(const void *p1, const void *p2) { // This function signature is required
    person *person1 = p1;
    person *person2 = p2;

    return NPK_string_equals(&person1->name, NPK_string_data(&person2->name)) &&
            NPK_string_equals(&person1->surname, NPK_string_data(&person2->surname)) &&
            person1->age == person2->age;
}

// Indicate how to sort in ascenting order (for example by age)
int person_asc(const void *p1, const void *p2) { // This function signature is required
    person *person1 = p1;
    person *person2 = p2;

    if (person1->age == person2->age) {
        return 0; // 0 indicates equality
    } else if (person1->age > person2->age) {
        return 1; // 1 indicates that the first parameter (p1) is bigger
    } else {
        return -1; // -1 indicates that the second paramter (p2) is bigger
    }
}

// Indicate how to sort in descenting order (for example by age)
int person_desc(const void *p1, const void *p2) { // This function signature is required
    person *person1 = p1;
    person *person2 = p2;

    if (person1->age == person2->age) {
        return 0;
    } else if (person1->age > person2->age) {
        return -1; // Notice how we flipped the sign intentionally
    } else {
        return 1; // The sign was flipped here too
    }
}

int main(int argc, char **argv) {
    // Create an array of type person
    NPK_array people = NPK_array_new(person);

    // Create some person objects
    person person1 = {NPK_string_new("name1"), NPK_string_new("surname1"), 30};
    person person2 = {NPK_string_new("name2"), NPK_string_new("surname2"), 20};
    person person3 = {NPK_string_new("name3"), NPK_string_new("surname3"), 40};

    // Add them to the array
    NPK_array_add(person, &people, person1);
    NPK_array_add(person, &people, person2);
    NPK_array_add(person, &people, person3);

    // Notice how it will sort using person_asc
    // Now the array holds the people in order:
    // person2, person1, person3
    NPK_array_sort(person, &people, person_asc);

    // Notice how it will sort using person_desc
    // Now the array holds the people in order:
    // person3, person1, person2
    NPK_array_sort(person, &people, person_desc);

    // Create a copy of the array
    NPK_array people_copy = array_person_copy_new(&people);

    // Notice how it will check for equality using person_equals
    if (NPK_array_equals(person, &people, &people_copy, person_equals)) {
        // Well, this block will be executed :)
    }

    // Let's deallocate our person arrays
    array_person_del(&people_copy);
    array_person_del(&people);
}
```
I think these 6 examples are more than enough, hopefully you didn't loose track along the way. If you have any previous experience with C++ classes, these examples should look awefully too familiar with all the constructors and destructors you had to create. For more details you should probably look into the source code, I tried to keep it small and simple, so you should probably find what you are looking for.

Lastly, I didn't mention in my examples the NPK\_cast, NPK\_rvalue\_addr, NPK\_sizeof\_type, NPK\_sizeof\_member, NPK\_swap and the NPK\_char\_\* macros along with the 4 NPK\_file\_\* and all the (from and to) string conversion functions. So if you take a deep dive, take look at them too! That's it from me, I hope you find my little library useful!
