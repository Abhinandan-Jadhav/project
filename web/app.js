const API_BASE = ''; // same origin

async function getLocation() {
  return new Promise((resolve) => {
    if (!navigator.geolocation) return resolve({ lat: 0, lon: 0 });
    navigator.geolocation.getCurrentPosition(
      (pos) => {
        resolve({ lat: pos.coords.latitude, lon: pos.coords.longitude });
      },
      () => resolve({ lat: 0, lon: 0 }),
      { enableHighAccuracy: true, timeout: 5000, maximumAge: 10000 }
    );
  });
}

async function sendPanic() {
  const btn = document.getElementById('panic');
  const status = document.getElementById('status');
  const type = document.getElementById('type').value;
  const description = document.getElementById('desc').value.trim();

  btn.disabled = true;
  status.textContent = 'Sending alert...';

  const { lat, lon } = await getLocation();

  try {
    const res = await fetch(`${API_BASE}/api/panic`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ type, description, lat, lon })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.message || 'Request failed');
    status.textContent = `Alert sent (ID: ${data.id})`;
    document.getElementById('desc').value = '';
    await loadAlerts();
  } catch (e) {
    status.textContent = `Error: ${e.message}`;
  } finally {
    btn.disabled = false;
    setTimeout(() => (status.textContent = ''), 4000);
  }
}

function formatTime(iso) {
  const d = new Date(iso);
  return d.toLocaleString();
}

async function loadAlerts() {
  const container = document.getElementById('alerts');
  try {
    const res = await fetch(`${API_BASE}/api/alerts`);
    const list = await res.json();
    container.innerHTML = '';
    list.forEach((a) => {
      const div = document.createElement('div');
      div.className = 'alert-card';
      div.innerHTML = `
        <div><strong>${a.type.toUpperCase()}</strong>: ${a.description || '(no description)'}</div>
        <div class="alert-meta">
          ID: ${a.id} • ${formatTime(a.timestamp)} • (${a.lat.toFixed(5)}, ${a.lon.toFixed(5)})
        </div>
      `;
      container.appendChild(div);
    });
  } catch (e) {
    container.innerHTML = `<div class="alert-card">Failed to load alerts.</div>`;
  }
}

document.addEventListener('DOMContentLoaded', () => {
  document.getElementById('panic').addEventListener('click', sendPanic);
  loadAlerts();
  setInterval(loadAlerts, 5000);
});
