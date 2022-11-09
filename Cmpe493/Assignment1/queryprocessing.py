def query_process():
    new_dict = {}  # it will store dictionary read from "myIndex.txt" file
    with open("myIndex.txt") as h:
        for line in h:
            line = line.replace(":", "")  # it removes ":" from key
            line = line.replace("[", " ")  # it removes "[" from value
            line = line.replace("]", " ")  # it removes "]" from value
            line = line.replace(",", "")  # it removes "," from value
            value_list = line.split()  # it will split whole line and makes a list
            key = value_list[0]  # first element of the list is key
            # other elements are news id values of the key and ı added them to dictionary.
            for i in range(1, len(value_list)):
                if key in new_dict:
                    new_dict[key].append(int(value_list[i]))
                else:
                    new_dict[key] = [int(value_list[i])]

    input_query = input("Please enter query:")  # takes query from user
    input_list = input_query.split()  # Makes a list by separating the input word by word.

    first = new_dict[input_list[0]]  # first word to search and its corresponding ids
    result = list()  # stores results
    # I iterate over the list by taking two new words from input. After making first boolean operation I am taking
    # next operand and next word and calculate this results an so on.
    for i in range(0, len(input_list) - 1, 2):
        if i == 0:
            result = first
        oper = input_list[i + 1]  # operands "AND" "OR" "NOT"
        second = new_dict[input_list[i + 2]]  # next word

        if oper == "AND":
            len_res = len(result)
            len_sec = len(second)
            a, b = 0, 0
            temp = list()
            # this loop runs until one list is finished
            while a < len_res and b < len_sec:
                if result[a] == second[b]:
                    temp.append(result[a])  # if ids are the same then it adds the value to result list.
                    a += 1
                    b += 1
                elif result[a] < second[b]:
                    a += 1
                elif result[a] > second[b]:
                    b += 1
            result = temp
        # it will combine two list and eliminates duplicates.
        elif oper == "OR":
            result_as_set = set(result)
            union = result_as_set.union(second)
            result = list(union)
        # It will remove ids of Not words ids from result
        elif oper == "NOT":
            result_as_set = set(result)
            diff = result_as_set.difference(second)
            result = list(diff)
        result.sort()  # it sorts the list in ascending order

    print(result)  # prints results.


query_process()
