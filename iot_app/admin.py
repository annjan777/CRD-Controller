# admin.py
from django.contrib import admin
from .models import ReceivedMessage

class ReceivedMessageAdmin(admin.ModelAdmin):
    list_display = ('user', 'topic', 'message', 'timestamp')
    search_fields = ('user__username', 'topic', 'message')

admin.site.register(ReceivedMessage, ReceivedMessageAdmin)
