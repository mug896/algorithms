# algorithms


binary search tree 는 데이터 입력, 검색, 삭제를 하는데 `O(log n)` 시간이 걸리기 때문에
데이터가 많이 증가하더라도 큰 차이가 나지 않습니다.
단순히 name 을 이용해 빠르게 value 를 검색할 때는 hash table 이 좋지만 tree 의 장점은
데이터가 정렬되어 저장된다는 점입니다.
따라서 따로 sorting 을 하지 않아도 tree 를 inorder 방식으로 출력하면 정렬된 결과를 얻을 수 있고,
tree 의 왼쪽 끝으로 가면 최소값이, 오른쪽 끝으로 가면 최대값이 나오게 됩니다.
이와 같은 tree 의 성질을 활용하면 특정 range 가 overlap 되는지 알아볼 수 있는 interval tree
같은 것도 만들 수 있습니다.


binary search tree 는 데이터를 입력한 후에 아무런 처리를 하지 않기 때문에
입력되는 값이 sequential 하게 이어질 경우 tree 가 한쪽으로 쏠리게 되어 성능이 `O(n)` 까지
떨어질 수 있습니다.
따라서 데이터를 입력, 삭제할 때마다 tree 의 균형을 맞춰주는 것이 필요한데
여기에는 대표적으로 redblack tree 와 avl tree 가 있습니다.
redblack tree 는 avl tree 와 같이 완벽하게 균형이 맞지는 않지만 대신 입력, 삭제 시 속도가
빠릅니다. ( 그리고 실제 검색 속도도 비슷합니다. )


위의 파일 목록에서 `avl_recursive.c` 는 recursion 을 이용해 구현한 것인데 recursion 을
이용하면 코드가 간결하고 보기가 좋지만 속도는 좀 떨어집니다.
따라서 자료구조를 만들 때는 가능하면 loop 문과 필요할 경우 stack 을 사용해 구현하는 것이
속도에 좋습니다.


위의 코드에 대한 자세한 설명은 아래 사이트에서 볼 수 있습니다.  
소스코드도 예제에 있는 것인데 typo 오류가 있어서 수정을 하였습니다.

https://www.codesdope.com/course/data-structures-introduction/


```sh
#
# randoms.h 파일은 데이터 입력에 사용하기 위한 파일로 
# 1 ~ 1,000,000 숫자를 shuf 명령으로 섞어서 array 로 저장한 것입니다.
#

$ gcc -O2 redblack_tree.c

$ ./a.out 
first node : 1
last node : 1000000
prev node : 214095
tree height : 24
node count all : 1000000
node count 2degree : 428414
node count 1degree : 143171
node count leaf : 428415
node count internal : 571585
sum all node data : 1784293664
The tree is BINARY SEARCH tree
The tree is NOT FULL binary tree
The tree is NOT PERFECT binary tree
The tree is NOT COMPLETE binary tree
=========================================
1000000 Insert DONE : 0.61283 seconds
1000000 Search DONE : 0.48632 seconds
1000000 Delete DONE : 0.22706 seconds
Insert + Search + Delete : 1.32621 seconds
```
