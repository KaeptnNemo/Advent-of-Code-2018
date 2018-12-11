
# read all number into an array
def all_numbers(fname):
    f = open(fname)
    return [int(line) for line in f]

# apply the frequency changes in @changes
# starting with frequency @freq
# and having the frequencies @inital_set that were already seen
def one_pass(inital_set, changes, inital_freq):
    freq   = inital_freq
    so_far = inital_set
    for change in changes:
        freq = freq + change
        if freq in so_far:
            return (so_far, freq, freq)
        else:
            so_far.add(freq)
    return (so_far, freq, None)

# apply one_pass until a frequency is found twice
def first_twice(changes):
    (tmp_set,tmp_freq,result) = one_pass({0}, changes, 0)
    while result == None:
        (tmp_set,tmp_freq,result) = one_pass(tmp_set,changes, tmp_freq)
    return result

print(first_twice(all_numbers("input.txt")))