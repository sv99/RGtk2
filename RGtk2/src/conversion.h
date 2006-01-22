#include "RGtk2.h"

typedef void (*RPointerFinalizer)(void *ptr);
typedef void* (*ElementConverter)(void *element);

/***** ARRAY CONVERSION MACROS ******/

/* converts an array, taking the reference of each element, so that conversion
	functions taking a pointer parameter will work (array elements are values) */
#define asRArrayRef(array, converter) \
({ \
    asRArray(&array, converter); \
})

#define asRArrayRefWithSize(array, converter, n) \
({ \
    asRArrayWithSize(&array, converter, n); \
})
/* converts an array directly using the conversion function to an R list */
#define asRArray(array, converter) \
({ \
    _asRArray(array, converter, LIST); \
})

#define asRArrayWithSize(array, converter, n) \
({ \
    _asRArrayWithSize(array, converter, n, LIST); \
})

/* converts primitive (numeric, integer, logical) arrays to R vectors */
#define _asRPrimArray(array, TYPE) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    _asRPrimArrayWithSize(array, n-1, TYPE); \
})

#define _asRPrimArrayWithSize(array, n, TYPE) \
({ \
    int i; \
    USER_OBJECT_ s_obj; \
    PROTECT(s_obj = NEW_ ## TYPE(n)); \
\
    for (i = 0; i < n; i++) { \
        TYPE ## _POINTER(s_obj)[i] = array[i]; \
	} \
\
    UNPROTECT(1); \
    s_obj; \
})

/* core converter, for converting string arrays and other arrays of pointer types */
#define _asRArray(array, converter, TYPE) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    _asRArrayWithSize(array, converter, n-1, TYPE); \
})

#define _asRArrayWithSize(array, converter, n, TYPE) \
({ \
    int i; \
    USER_OBJECT_ s_obj; \
    PROTECT(s_obj = NEW_ ## TYPE(n)); \
\
    for (i = 0; i < n; i++) { \
        SET_VECTOR_ELT(s_obj, i, converter(array[i])); \
	} \
\
    UNPROTECT(1); \
    s_obj; \
})

/* Below are primitive array -> R vector converters */

#define asRStringArray(array) \
({ \
    _asRArray(array, COPY_TO_USER_STRING, CHARACTER); \
})

#define asRStringArrayWithSize(array, n) \
({ \
    _asRArrayWithSize(array, COPY_TO_USER_STRING, n, CHARACTER); \
})

#define asRIntegerArray(array) \
({ \
    _asRPrimArray(array, INTEGER); \
})

#define asRIntegerArrayWithSize(array, size) \
({ \
    _asRPrimArrayWithSize(array, size, INTEGER); \
})

#define asRNumericArray(array) \
({ \
    _asRPrimArray(array, NUMERIC); \
})

#define asRNumericArrayWithSize(array, size) \
({ \
    _asRPrimArrayWithSize(array, size, NUMERIC); \
})

#define asRLogicalArray(array) \
({ \
    _asRPrimArray(array, LOGICAL); \
})

#define asRLogicalArrayWithSize(array, size) \
({ \
    _asRPrimArrayWithSize(array, size, LOGICAL); \
})

/* for converting each element to an R pointer of a specified class 
	-- I don't think this is ever used */ 
#define toRPointerArray(array, type) \
({ \
    toRPointerWithFinalizerArray(array, type); \
})
#define toRPointerArrayWithSize(array, type, n) \
({ \
    toRPointerWithFinalizerArrayWithSize(array, type, NULL, n); \
})

/* for converting elements to R pointers of a specified class with a special finalizer 
	- only used like once */
#define toRPointerWithFinalizerArray(array, type, finalizer) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    asRPointerWithFinalizerArrayWithSize(array, type, finalizer, n-1); \
})
#define toRPointerWithFinalizerArrayWithSize(array, type, finalizer, n) \
({ \
    int i; \
    USER_OBJECT_ s_array; \
    PROTECT(s_array = NEW_LIST(n)); \
\
    for (i = 0; i < n; i++) { \
        SET_VECTOR_ELT(s_array, i, toRPointerWithFinalizer(array[i], type, finalizer)); \
    } \
\
    UNPROTECT(1); \
    s_array; \
})

/* converts each element to a ref'd R pointer (ie, they're GObjects) 
	- used only a couple of times */
#define toRPointerWithRefArray(array, type) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    asRPointerWithRefArrayWithSize(array, type, n-1); \
})
#define toRPointerWithRefArrayWithSize(array, type, n) \
({ \
    int i; \
    USER_OBJECT_ s_array; \
    PROTECT(s_array = NEW_LIST(n)); \
\
    for (i = 0; i < n; i++) { \
        SET_VECTOR_ELT(s_array, i, toRPointerWithRef(array[i], type)); \
    } \
\
    UNPROTECT(1); \
    s_array; \
})

/* this is used when there is an array of struct values that need to be copied
	into separate areas in memory so that they can be individually finalized 
	- used maybe once */
#define asRStructArray(array, type) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    asRStructArrayWithSize(array, type, n-1); \
})
#define asRStructArrayWithSize(array, type, n) \
({ \
    int i; \
    USER_OBJECT_ s_array; \
    PROTECT(s_array = NEW_LIST(n)); \
\
    for (i = 0; i < n; i++) { \
        typeof(array) ptr = g_malloc(sizeof(typeof(array[i]))); \
        memcpy(ptr, array+i, sizeof(typeof(array[i]))); \
        SET_VECTOR_ELT(s_array, i, toRPointerWithFinalizer(ptr, type, g_free)); \
    } \
\
    UNPROTECT(1); \
    s_array; \
})

/* for converting enum elements of a given type */
#define asREnumArray(array, type) \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    asREnumArrayWithSize(array, type, n-1); \
})
#define asREnumArrayWithSize(array, type, n) \
({ \
    int i; \
    USER_OBJECT_ s_array; \
    PROTECT(s_array = NEW_LIST(n)); \
\
    for (i = 0; i < n; i++) { \
        SET_VECTOR_ELT(s_array, i, asREnum(array[i], type)); \
    } \
\
    UNPROTECT(1); \
    s_array; \
})

/* now from R to C */

#define asCArrayRef(s, type, converter) \
({ \
    asCArray(s, type, * converter); \
})
#define asCArray(s_array, type, converter) \
({ \
    int i; \
\
    type* array = (type*)R_alloc(GET_LENGTH(s_array), sizeof(type)); \
\
    for (i = 0; i < GET_LENGTH(s_array); i++) { \
        array[i] = (type)converter(VECTOR_ELT(s_array, i)); \
    } \
\
    array; \
})

#define asCEnumArray(s_array, type, code) \
({ \
    int i; \
\
    type* array = (type*)R_alloc(GET_LENGTH(s_array), sizeof(type)); \
\
    for (i = 0; i < GET_LENGTH(s_array); i++) { \
        array[i] = asCEnum(VECTOR_ELT(s_array, i), code); \
    } \
\
    array; \
})

gboolean asCLogical(USER_OBJECT_ s_log);
int asCInteger(USER_OBJECT_ s_int);
double asCNumeric(USER_OBJECT_ s_num);
char * asCString(USER_OBJECT_ s_str);
char asCCharacter(USER_OBJECT_ s_char);

USER_OBJECT_ asRLogical(Rboolean);
USER_OBJECT_ asRInteger(int);
USER_OBJECT_ asRNumeric(double);
USER_OBJECT_ asRCharacter(char c);
USER_OBJECT_ asRString(const char *);
USER_OBJECT_ asRGdkAtom(GdkAtom);

USER_OBJECT_ asCGenericData(USER_OBJECT_ obj);
char ** asCStringArray(USER_OBJECT_ svec); /* NULL TERMINATES */

USER_OBJECT_ asREnum(int value, GType etype);
USER_OBJECT_ R_createEnum(int value, const char *enumName);
USER_OBJECT_ asRFlag(guint value, GType ftype);
USER_OBJECT_ R_createFlag(int, const char *);

guint asCFlag(USER_OBJECT_ s_flag, GType ftype);
gint asCEnum(USER_OBJECT_ s_enum, GType etype);

USER_OBJECT_ toRPointerWithFinalizer(void *val, const gchar *typeName, RPointerFinalizer finalizer);
USER_OBJECT_ toRPointerWithRef(void *val, const char *type);

#define toRPointerWithCairoRef(ptr, name, type) \
({ \
	type ## _reference(ptr); \
	toRPointerWithFinalizer(ptr, name, (RPointerFinalizer) type ## _destroy); \
})

USER_OBJECT_ toRPointer(void*, const char *name);
void RGtk_finalizer(USER_OBJECT_ extptr);

void *getPtrValue(USER_OBJECT_);

/* GObject */
int R_setGValueFromSValue(GValue *, USER_OBJECT_);
GValue* createGValueFromSValue(USER_OBJECT_ sval);
gboolean initGValueFromSValue(USER_OBJECT_ sval, GValue *raw);
gboolean initGValueFromVector(USER_OBJECT_ sval, gint n, GValue *raw);
USER_OBJECT_ asRGValue(GValue *);
GValue* asCGValue(USER_OBJECT_);
USER_OBJECT_ asRGType(GType);
GParamSpec* asCGParamSpec(USER_OBJECT_ s_spec);
USER_OBJECT_ asRGParamSpec(GParamSpec* spec);
GClosure* asCGClosure(USER_OBJECT_ s_closure);
USER_OBJECT_ asRGClosure(GClosure *closure);

/* GLib */
USER_OBJECT_ asRGQuark(GQuark val);
GTimeVal* asCGTimeVal(USER_OBJECT_ s_timeval);
GString* asCGString(USER_OBJECT_ s_string);
GList* asCGList(USER_OBJECT_ s_list);
USER_OBJECT_ asRGList(GList *glist, const gchar* type);
USER_OBJECT_ asRGListWithRef(GList *glist, const gchar* type);
USER_OBJECT_ asRGListWithSink(GList *glist, const gchar* type);
USER_OBJECT_ asRGListWithFinalizer(GList *glist, const gchar* type, RPointerFinalizer finalizer);
USER_OBJECT_ asRGListConv(GList *glist, ElementConverter converter);
GSList* asCGSList(USER_OBJECT_ s_list);
USER_OBJECT_ asRGSList(GSList *gslist, const gchar* type);
USER_OBJECT_ asRGSListWithRef(GSList *gslist, const gchar* type);
USER_OBJECT_ asRGSListWithSink(GSList *gslist, const gchar* type); 
USER_OBJECT_ asRGSListWithFinalizer(GSList *gslist, const gchar* type, RPointerFinalizer finalizer);
USER_OBJECT_ asRGSListConv(GSList *gslist, ElementConverter converter);
USER_OBJECT_ asRGError(GError *error);

/* GDK */
USER_OBJECT_ asRGdkAtom(GdkAtom val);
GdkAtom asCGdkAtom(USER_OBJECT_ s_atom);
GdkAtom* asCGdkAtomArray(USER_OBJECT_ s_atoms);
GdkGeometry* asCGdkGeometry(USER_OBJECT_ s_geom, GdkWindowHints *hints);
GdkGCValues* asCGdkGCValues(USER_OBJECT_ s_values, GdkGCValuesMask *mask);
GdkWindowAttr* asCGdkWindowAttr(USER_OBJECT_ s_attr, GdkWindowAttributesType *mask);
USER_OBJECT_ asRGdkTimeCoord(GdkTimeCoord *coord, int num_axes);
GdkRectangle* asCGdkRectangle(USER_OBJECT_ s_rect);
USER_OBJECT_ asRGdkRectangle(GdkRectangle *rect);
GdkRgbCmap* asCGdkRgbCmap(USER_OBJECT_ s_cmap);
USER_OBJECT_ asRGdkRgbCmap(GdkRgbCmap *map);
GdkKeymapKey* asCGdkKeymapKey(USER_OBJECT_ s_key);
USER_OBJECT_ asRGdkKeymapKey(GdkKeymapKey* key);
GdkPoint* asCGdkPoint(USER_OBJECT_ s_point);
USER_OBJECT_ asRGdkPoint(GdkPoint *point);
GdkSegment* asCGdkSegment(USER_OBJECT_ s_segment);
GdkColor* asCGdkColor(USER_OBJECT_ s_color);
USER_OBJECT_ asRGdkColor(GdkColor* color);
USER_OBJECT_ asRGdkNativeWindow(GdkNativeWindow window);
GdkNativeWindow asCGdkNativeWindow(USER_OBJECT_ s_window);
USER_OBJECT_ asRGdkEvent(GdkEvent *event);
USER_OBJECT_ toRGdkEvent(GdkEvent *event, gboolean finalize);
USER_OBJECT_ toRGdkFont(GdkFont *font);
GdkTrapezoid * asCGdkTrapezoid(USER_OBJECT_ s_trapezoid);
USER_OBJECT_ asRGdkGCValues(GdkGCValues *values);
USER_OBJECT_ asRGdkEvent(GdkEvent *event);
GdkSpan* asCGdkSpan(USER_OBJECT_ s_span);

/* GTK */

GtkTargetEntry* asCGtkTargetEntry(USER_OBJECT_ s_entry);
GtkFileFilterInfo* asCGtkFileFilterInfo(USER_OBJECT_ s_info);
GtkSettingsValue* asCGtkSettingsValue(USER_OBJECT_ s_value);
GtkStockItem* asCGtkStockItem(USER_OBJECT_ s_item);
USER_OBJECT_ asRGtkStockItem(GtkStockItem *item);
GtkItemFactoryEntry* asCGtkItemFactoryEntry(USER_OBJECT_ s_entry);
GtkItemFactoryEntry* asCGtkItemFactoryEntry2(USER_OBJECT_ s_entry);
GtkItemFactoryEntry* R_createGtkItemFactoryEntry(USER_OBJECT_ s_entry, guint cbtype);
GtkAllocation* asCGtkAllocation(USER_OBJECT_ s_alloc);
USER_OBJECT_ asRGtkAllocation(GtkAllocation* alloc);
USER_OBJECT_ toRPointerWithSink(void *val, const char *type);

/* ATK */

AtkAttributeSet* asCAtkAttributeSet(USER_OBJECT_ s_set);
AtkAttribute* asCAtkAttribute(USER_OBJECT_ s_attr);
USER_OBJECT_ asRAtkAttributeSet(AtkAttributeSet* set);
USER_OBJECT_ asRAtkAttribute(AtkAttribute* attr);
AtkTextRectangle* asCAtkTextRectangle(USER_OBJECT_ s_rect);
USER_OBJECT_ asRAtkTextRectangle(AtkTextRectangle *rect);
USER_OBJECT_ asRAtkTextRange(AtkTextRange *range);

/* Pango */

PangoRectangle* asCPangoRectangle(USER_OBJECT_ s_rect);
USER_OBJECT_ asRPangoRectangle(PangoRectangle *rect);
USER_OBJECT_ asRPangoAttribute(PangoAttribute *attr);
USER_OBJECT_ toRPangoAttribute(PangoAttribute *attr, gboolean finalize);

/* Cairo */
USER_OBJECT_ asRCairoPath(cairo_path_t *path);
cairo_path_t * asCCairoPath(USER_OBJECT_ s_path);
cairo_glyph_t * asCCairoGlyph(USER_OBJECT_ s_glyph);
