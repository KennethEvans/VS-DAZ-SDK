#include <QtCore/QObject>
#include "dzmaterial.h"


bool compareMaterials(const QObject *obj1, const QObject *obj2) {
	DzMaterial *material1 =  (DzMaterial *)obj1;
	DzMaterial *material2 =  (DzMaterial *)obj2;
	return material1->getName() < material2->getName();
}

void sortMaterialsByName(QObjectList &in, QObjectList &out) {
	int nItems = in.count();
	out.empty();
	for(int i=0; i < nItems; i++) {
		out.append(in[i]);
	}
	qSort(out.begin(), out.end(), compareMaterials);
}
