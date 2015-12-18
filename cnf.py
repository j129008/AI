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

print postfix

stack = []
while len(postfix) > 0:
    if postfix[0] not in ops:
        stack = [postfix.pop(0)] + stack
    else:
        op2 = stack.pop(0)
        op1 = stack.pop(0)
        stack.insert(0, [op1, op2, postfix[0]])
        postfix.pop(0)

print stack
