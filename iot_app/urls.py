# iot_app/urls.py

from django.urls import path
from . import views

urlpatterns = [
    path('publish/', views.publish_to_iot, name='publish_to_iot'),
    path('logs/', views.view_logs, name='view_logs'),
    path('about/', views.about, name='about'),
    path('home/', views.home, name='home'),
    path('update/', views.update, name='update'),
    path('contact/', views.contact, name='contact'),
    path('status/',views.status,name='status'),
    path('get_status/',views.get_status,name='get_status'),
]
