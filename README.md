# cpp_PJC_database
cpp_PJC_database is a **simple database project** created in C++ during my studies at Polish-Japanese Academy of Information Technology at Warsaw in 2024.

## Database
Database system supports table structure:

- table - represents entity
- column - represents attribute - available types are strings, integers and null
- row - represents data

## Language
This project supports a custom-made language created by myself. 

It supports pipelining mechanism - it means that every command passes result to another. This way you can create complex instructions in fewer lines.\
Example: `get_table(employees) add_column(id, int) add_column(name, string) add_column(surname, string)`

You should seperate commands using spaces.

### Commands
Basic commands
 - `quit()` - exits from application (alias: `exit()`)
 - `create_db(filename)` - creates new database with given filename
 - `connect_db(filename)` - loads database from file
 - `save_db()` - saves database
 - `export_db(filename)` - exports current database to a file under given filename
 - `get_table(table1, table2, ...)` - gathers tables for further commands (alias: `from(...)`)
 - `create_table(table1)` - creates new table and gathers it for further commands

Commands below require either `get_table(...)` or `create_table(...)` used before:
 - `add_column(name, type, [nullable])` - dodaje kolumnę
    - _name_ - column name as string
    - _type_ - field type (_int/string/null_)
    - _[nullable]_ - optional argument, marks column as nullable
 - `get_column(name)` - gathers column with passed name
 - `add_row(...)` - adds new row to the tables - you need to pass values in the same order as columns are in the table (alias: `insert(...)`)
 - `select(column1, column2, ...)` - filters columns shown in result
 - `where(...)` - adds condition to the result that all rows have to fulfill
    - this language currently supports only following binary operators: `>`, `>=`, `==`, `<=`, `<`, `!=`
    - empty `where()` is equal to `where(1==1)` - this way you can for example remove or update all rows in result
 - `sort_by(column, [asc/desc])` - sorts row by given column (alias: `order_by(...)`)
    - if `[asc/desc]` is omitted, asceding order is applied
    - _asc_ - ascending, _desc_ - descending

 Commands below require either any `get_` or `create_table(...)` command used before:
 - `info()` - display information about currently selected object (database/column/result)
 - `print()` - displays result of operation in command line
 - `rename(new_name)` - renames currently selected object (table/column)
 - `remove()` - removes currently selected objects (tables/columns/rows) (alias: `drop()`)
    - attention - you can remove rows from only one table at a time

Commands below require using `where(...)` and either `get_table(...)` or `create_table(...)` commands before:
 - `update(column, value)` - updates values at given column in all rows that meet given conditions

### Disclaimers
 - interpreter does not display DQL results - you need to use `print()` command always
 - if you want to enter null value, please write it uppercased - `NULL`
 - please use `==` and `!=` when comparing values to NULL
 - you may need to use `table.column` syntax if you want to use column that exists in many tables under it's name

### Example

`get_table(employees) select(ID,name,surname) where(salary>=5000) print()` 

Instruction above prints out all IDs, names and surnames of employees that have salary more or equal than 5000.
