#pragma once
int getMid(int* array, int ia, int ib, int ic)
{
	if (array[ia] > array[ib])
	{
		if (array[ib] > array[ic])
			return ib;
		else
			return array[ia] < array[ic] ? ia : ic;
	}
	else
	{
		if (array[ia] > array[ic])
			return ia;
		else
			return array[ib] < array[ic] ? ib : ic;
	}
}
static int countQuickSortTail = 0;
void quickSortTail(int* array, int stIdx, int endIdx, int depth = 0)
{
	countQuickSortTail = depth > countQuickSortTail ? depth : countQuickSortTail;
	while (stIdx < endIdx)
	{
		//int tMidIdx = stIdx + rand() % (endIdx - stIdx);
		int tMidIdx = stIdx;
		int mid = array[tMidIdx];
		array[tMidIdx] = array[stIdx];
		array[stIdx] = mid;

		int curIdx = stIdx;
		for (int i = stIdx + 1; i < endIdx; ++i)
		{
			if (array[i] <= mid)
			{
				++curIdx;
				int temp = array[curIdx];
				array[curIdx] = array[i];
				array[i] = temp;
			}
		}
		array[stIdx] = array[curIdx];
		array[curIdx] = mid;
		quickSortTail(array, stIdx, curIdx, depth + 1);

		stIdx = curIdx + 1;
	}
}

static int countQuickSortMid = 0;
void quickSortMid(int* array, int stIdx, int endIdx, int depth = 0)
{
	if (stIdx >= endIdx)
	{
		countQuickSortMid = depth > countQuickSortMid ? depth : countQuickSortMid;
		return;
	}

	//cout << "idx " << stIdx << " " << (stIdx + endIdx) / 2 << " " << endIdx << endl;
	//cout <<"pickMid " << array[stIdx] <<" "<< array[(stIdx + endIdx) / 2] << " " << array[endIdx]<<endl;
	//int tMidIdx = getMid(array, stIdx, (stIdx + endIdx) / 2, endIdx);
	int tMidIdx = stIdx + rand() % (endIdx - stIdx);
	//cout << array[tMidIdx] << endl;

	//int tMidIdx = stIdx;
	int mid = array[tMidIdx];
	array[tMidIdx] = array[stIdx];
	array[stIdx] = mid;

	int curIdx = stIdx;
	for (int i = stIdx + 1; i < endIdx; ++i)
	{
		if (array[i] <= mid)
		{
			++curIdx;
			int temp = array[curIdx];
			array[curIdx] = array[i];
			array[i] = temp;
		}
	}
	array[stIdx] = array[curIdx];
	array[curIdx] = mid;
	quickSortMid(array, stIdx, curIdx, depth + 1);
	quickSortMid(array, curIdx + 1, endIdx, depth + 1);
}

static int countQuickSort = 0;
void quickSort(int* array, int stIdx, int endIdx, int depth = 0)
{
	if (stIdx >= endIdx)
	{
		countQuickSort = depth > countQuickSort ? depth : countQuickSort;
		return;
	}
	int tMidIdx = stIdx;
	int mid = array[tMidIdx];
	array[tMidIdx] = array[stIdx];
	array[stIdx] = mid;

	int curIdx = stIdx;
	for (int i = stIdx + 1; i < endIdx; ++i)
	{
		if (array[i] <= mid)
		{
			++curIdx;
			int temp = array[curIdx];
			array[curIdx] = array[i];
			array[i] = temp;
		}
	}
	array[stIdx] = array[curIdx];
	array[curIdx] = mid;
	quickSort(array, stIdx, curIdx, depth + 1);
	quickSort(array, curIdx + 1, endIdx, depth + 1);
}