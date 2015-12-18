ops = { 'iff': 1, 'imp': 2, 'or': 3, 'and': 4, 'not': 5, '(': 0, ')':6 }
opStack = []
postfix = []

text = raw_input("input your logic: ")
tokens = text.split(" ")

while len(tokens) > 0 :
    token = tokens.pop(0)
    if token == '(':
        opStack = ['('] + opStack
    elif token == ')':
        while opStack[0] != '(' :
            postfix.append(opStack.pop(0))
        opStack.pop(0)
    elif token in ops:
        if len(opStack) == 0 :
            opStack = [token] + opStack
            continue
        while ops[token] <= ops[opStack[0]]:
            postfix.append(opStack.pop(0))
            if len(opStack) == 0 :
                break
        opStack = [token] + opStack
    else:
        postfix.append(token)


while len(opStack) > 0 :
    postfix.append(opStack.pop(0))

# print postfix


stack = []
while len(postfix) > 0:
    if postfix[0] not in ops:
        stack = [postfix.pop(0)] + stack
    elif postfix[0] == 'not':
        op1 = stack.pop(0)
        stack.insert(0, [op1, postfix[0]])
        postfix.pop(0)
    else:
        op2 = stack.pop(0)
        op1 = stack.pop(0)
        stack.insert(0, [op1, op2, postfix[0]])
        postfix.pop(0)

parseTree = stack[0]

def rmIff(logic):
    if isinstance(logic, str):
        ans = logic
    else:
        op = logic[-1]
        if op == 'not':
            tree = rmIff(logic[0])
            ans = [tree, 'not']
        else:
            tree1 = rmImp(logic[0])
            tree2 = rmImp(logic[1])
            if op == 'iff':
                ans = [ [tree1, tree2, 'imp'], [tree2, tree1, 'imp'], 'and' ]
            else:
                ans = [tree1, tree2, op]
    return ans

def rmImp(logic):
    if isinstance(logic, str):
        ans = logic
    else:
        op = logic[-1]
        if op == 'not':
            tree = rmImp(logic[0])
            ans = [tree, 'not']
        else:
            tree1 = rmImp(logic[0])
            tree2 = rmImp(logic[1])
            if op == 'imp':
                ans = [ [tree1, 'not'], tree2, 'or']
            else:
                ans = [ tree1, tree2, op ]
    return ans

parseTree = rmIff(parseTree)
parseTree = rmImp(parseTree)
print parseTree

