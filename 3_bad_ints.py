import random

# Generate a large array of random integers
n = 1000
arr = [random.randint(-1000, 1000) for _ in range(n)]
target = random.randint(-1000, 1000)

print("Input array size:", n)
print("Target:", target)

# Sorting and two-pointer technique
def three_sum(arr, target):
    arr.sort()
    triplets = []

    for i in range(len(arr) - 2):
        if i > 0 and arr[i] == arr[i - 1]:
            continue

        left = i + 1
        right = len(arr) - 1

        while left < right:
            current_sum = arr[i] + arr[left] + arr[right]

            if current_sum == target:
                triplets.append([arr[i], arr[left], arr[right]])
                left += 1
                right -= 1

                while left < right and arr[left] == arr[left - 1]:
                    left += 1

                while left < right and arr[right] == arr[right + 1]:
                    right -= 1

            elif current_sum < target:
                left += 1
            else:
                right -= 1

    return triplets

# Hash table approach
def three_sum_hash(arr, target):
    triplets = set()
    n = len(arr)

    for i in range(n):
        first = arr[i]
        seen = set()

        for j in range(i+1, n):
            second = arr[j]
            potential_match = target - first - second

            if potential_match in seen:
                triplet = tuple(sorted([first, second, potential_match]))
                triplets.add(triplet)
            else:
                seen.add(second)

    return [list(triplet) for triplet in triplets]

print("\nSorting and Two-Pointer Technique:")
triplets = three_sum(arr, target)
print("Number of triplets found:", len(triplets))
#print(triplets)  #Uncomment to see the results


print("\nHash Table Approach:")
triplets_hash = three_sum_hash(arr, target)
print("Number of triplets found:", len(triplets_hash))
#print(triplets_hash)   #Uncomment to see the results
