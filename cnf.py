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

def postfixToInfix(logic):
    if isinstance(logic, str):
        ans = logic
    else:
        op = logic[-1]
        if op == 'not':
            ans = '( not ' + postfixToInfix(logic[0]) + ' )'
        else:
            ans = '( ' + postfixToInfix(logic[0]) + ' ' + op + ' ' + postfixToInfix(logic[1]) + ' )'
    return ans

print postfixToInfix(parseTree)

def moveNegations(logic):
    if isinstance(logic, str):
        ans = logic
    else:
        op = logic[-1]
        if op == 'not':
            arg = logic[0]
            if isinstance(arg, str) :
                ans = "-" + arg
            else:
                innerOp = arg[-1]
                if innerOp == 'not':
                    ans = moveNegations(arg[0])
                else:
                    tree1 = moveNegations([arg[0], 'not'])
                    tree2 = moveNegations([arg[1], 'not'])
                    dual = { 'and': 'or', 'or': 'and' }
                    ans = [ tree1, tree2, dual[innerOp] ]
        else:
            tree1 = moveNegations(logic[0])
            tree2 = moveNegations(logic[1])
            ans = [tree1, tree2, op]
    return ans

parseTree = moveNegations(parseTree)
print postfixToInfix(parseTree)

def distribute_Or(p1, p2):
    if isinstance(p1, list) and p1[-1] == 'and' :
        ans = [distribute_Or(p1[0],p2), distribute_Or(p1[1],p2), 'and']
    elif  isinstance(p2, list) and p2[-1] == 'and' :
        ans = [distribute_Or(p1,p2[0]), distribute_Or(p1,p2[1]), 'and']
    else :
        ans = [p1, p2, 'or']
    return ans

def makeCNF(logic):
    if isinstance(logic, str):
        ans = logic
    else:
        op = logic[-1]
        tree1 = makeCNF(logic[0])
        tree2 = makeCNF(logic[1])
        if op == 'and' :
            ans = [tree1, tree2, 'and']
        else :
            ans = distribute_Or(tree1, tree2)
    return ans

parseTree = makeCNF(parseTree)
print postfixToInfix(parseTree)
